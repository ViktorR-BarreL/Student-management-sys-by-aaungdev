#include <iostream>  // Для работы с потоками ввода-вывода (cin, cout, cerr)
#include <cstring>   // Для работы с C-строками (strcpy, strcmp и т.д.)
#include <iomanip>   // Для манипуляций с выводом (setw, setfill)
#include <fstream>   // Для работы с файловыми потоками (ofstream, ifstream)
#include <Windows.h> // Для работы с консолью на русском языке

// Используем стандартное пространство имен, чтобы не писать std:: перед каждым объектом
using namespace std;

// --- Глобальные константы ---
const int MAX_STUDENTS = 100;                 // Максимальное количество студентов в базе
const int MAX_NAME_LENGTH = 50;               // Максимальная длина имени студента
const int MAX_COURSE_LENGTH = 50;             // Максимальная длина названия курса

// --- Структура для хранения данных о студенте ---
struct Student {
    int id;                         // Уникальный идентификатор студента
    char name[MAX_NAME_LENGTH];     // Имя студента
    int age;                        // Возраст студента
    char course[MAX_COURSE_LENGTH]; // Название курса, на котором обучается студент
};

// --- Глобальные переменные ---
Student students[MAX_STUDENTS]; // Массив для хранения всех записей о студентах
int studentCount = 0;           // Текущее количество студентов в базе

// --- Работа с файлом ---
const char* FILE_NAME = "students.dat"; // Имя файла для сохранения и загрузки данных (бинарный)

/*
Сохраняет все текущие записи о студентах в бинарный файл.
Функция перезаписывает файл каждый раз при вызове.
 */
void saveToFile() {
    // Открываем файл для записи в бинарном режиме (ios::binary) и с удалением предыдущего содержимого (ios::trunc)
    ofstream out(FILE_NAME, ios::binary | ios::trunc);
    // Проверяем, успешно ли открылся файл
    if (!out) {
        cerr << "Ошибка открытия файла для записи." << endl;
        return; // Выходим из функции, если файл не открылся
    }
    // Записываем сначала количество студентов
    out.write(reinterpret_cast<char*>(&studentCount), sizeof(studentCount));
    // Затем записываем весь массив студентов
    out.write(reinterpret_cast<char*>(students), sizeof(Student) * studentCount);
    // Закрываем файл после завершения записи
    out.close();
}

/*
Загружает записи о студентах из бинарного файла.
Вызывается при запуске программы для восстановления данных.
 */
void loadFromFile() {
    // Открываем файл для чтения в бинарном режиме
    ifstream in(FILE_NAME, ios::binary);
    // Проверяем, открылся ли файл. Если нет, это может означать, что программа запускается впервые.
    if (!in) {
        // файла может не быть при первом запуске — это нормально
        return; // Просто выходим, база данных будет пустой
    }
    // Считываем количество студентов
    in.read(reinterpret_cast<char*>(&studentCount), sizeof(studentCount));
    // Считываем данные студентов в массив
    in.read(reinterpret_cast<char*>(students), sizeof(Student) * studentCount);
    // Закрываем файл
    in.close();
}

/*
Выводит текст по центру консоли.
 * @param text Текст для вывода.
 * @param width Общая ширина поля для центрирования.
 */
void printCentered(const string& text, int width) {
    // Рассчитываем количество отступов слева
    int padding = (width - text.length()) / 2;
    // Печатаем отступы
    for (int i = 0; i < padding; ++i) {
        cout << " ";
    }
    // Печатаем сам текст
    cout << text << endl;
}

/*
Проверяет, является ли переданный ID уникальным.
 * @param id Идентификатор для проверки.
 * @return true, если ID уникален, false - в противном случае.
 */
bool isUniqueID(int id) {
    // Проходим по всем существующим студентам
    for (int i = 0; i < studentCount; i++) {
        // Если находим студента с таким же ID, возвращаем false
        if (students[i].id == id) {
            return false;
        }
    }
    // Если цикл завершился и совпадений не найдено, ID уникален
    return true;
}

/*
Добавляет новую запись о студенте в базу данных.
 */
void insertStudent() {
    // Проверяем, не достигнут ли лимит студентов
    if (studentCount >= MAX_STUDENTS) {
        printCentered("Список студентов полон.", 100);
        return;
    }

    int id;
    printCentered("Введите ID студента: ", 100);
    cin >> id;
    // Проверяем уникальность ID. Если ID уже существует, просим ввести другой.
    while (!isUniqueID(id)) {
        printCentered("Такой ID уже существует. Введите уникальный ID: ", 100);
        cin >> id;
    }
    students[studentCount].id = id;

    // Очищаем буфер ввода перед чтением строки
    cin.ignore();

    printCentered("Введите имя студента: ", 100);
    // Считываем строку с именем
    cin.getline(students[studentCount].name, MAX_NAME_LENGTH);

    printCentered("Введите возраст студента: ", 100);
    cin >> students[studentCount].age;
    // Проверяем корректность ввода возраста
    while (cin.fail() || students[studentCount].age <= 0 || students[studentCount].age > 100) {
        cin.clear(); // Сбрасываем флаги ошибок потока ввода
        cin.ignore(1000, '\n'); // Очищаем буфер ввода
        printCentered("Неверный возраст. Введите корректный возраст (1-100): ", 100);
        cin >> students[studentCount].age;
    }
    // Очищаем буфер ввода после чтения числа
    cin.ignore();

    printCentered("Введите название курса: ", 100);
    cin.getline(students[studentCount].course, MAX_COURSE_LENGTH);

    // Увеличиваем счетчик студентов
    studentCount++;
    // Сохраняем изменения в файл
    saveToFile();
    printCentered("Запись о студенте успешно добавлена.", 100);
}


/*
Отображает список всех студентов в базе данных.
 */
void viewAllStudents() {
    // Проверяем, есть ли записи для отображения
    if (studentCount == 0) {
        printCentered("Записи о студентах не найдены.", 100);
        return;
    }
    // Выводим заголовок таблицы
    printCentered("ID\tИмя\t\tВозраст\tКурс", 100);
    printCentered("-------------------------------------------", 100);
    // Проходим по всем студентам и выводим их данные
    for (int i = 0; i < studentCount; i++) {
        cout << setw(25) << setfill(' ') << students[i].id << "\t"
            << setw(25) << setfill(' ') << students[i].name << "\t\t"
            << setw(25) << setfill(' ') << students[i].age << "\t"
            << setw(25) << setfill(' ') << students[i].course << endl;
    }
}

/*
Удаляет запись о студенте по его ID.
 */
void deleteStudent() {
    int id;
    printCentered("Введите ID студента для удаления: ", 100);
    cin >> id;
    // Ищем студента с указанным ID
    for (int i = 0; i < studentCount; i++) {
        if (students[i].id == id) {
            // Когда студент найден, сдвигаем все последующие элементы массива влево на одну позицию
            for (int j = i; j < studentCount - 1; j++) {
                students[j] = students[j + 1];
            }
            // Уменьшаем общее количество студентов
            studentCount--;
            saveToFile(); // Сохраняем изменения в файл
            printCentered("Запись о студенте успешно удалена.", 100);
            return; // Выходим из функции
        }
    }
    // Если цикл завершился, а студент не найден
    printCentered("Студент с таким ID не найден.", 100);
}

/*
Обновляет данные существующего студента по его ID.
 */
void updateStudent() {
    int id;
    printCentered("Введите ID студента для обновления: ", 100);
    cin >> id;
    // Ищем студента для обновления
    for (int i = 0; i < studentCount; i++) {
        if (students[i].id == id) {
            // Очищаем буфер ввода
            cin.ignore();

            // Запрашиваем новые данные
            printCentered("Введите новое имя студента: ", 100);
            cin.getline(students[i].name, MAX_NAME_LENGTH);

            printCentered("Введите новый возраст студента: ", 100);
            cin >> students[i].age;
            // Проверяем корректность ввода возраста
            while (cin.fail() || students[i].age <= 0 || students[i].age > 100) {
                cin.clear();
                cin.ignore(1000, '\n');
                printCentered("Неверный возраст. Введите корректный возраст (1-100): ", 100);
                cin >> students[i].age;
            }
            cin.ignore();

            printCentered("Введите новый курс студента: ", 100);
            cin.getline(students[i].course, MAX_COURSE_LENGTH);

            saveToFile(); // Сохраняем обновленные данные в файл
            printCentered("Запись о студенте успешно обновлена.", 100);
            return;
        }
    }
    // Если студент не найден
    printCentered("Студент с таким ID не найден.", 100);
}

/*
Сортирует массив студентов по имени в алфавитном порядке.
Используется алгоритм пузырьковой сортировки.
 */
void sortStudentsByName() {
    // Реализация алгоритма пузырьковой сортировки
    for (int i = 0; i < studentCount - 1; i++) {
        for (int j = 0; j < studentCount - i - 1; j++) {
            // Сравниваем имена двух соседних студентов
            if (strcmp(students[j].name, students[j + 1].name) > 0) {
                // Если порядок неверный, меняем их местами
                Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }
    saveToFile(); // Сохраняем отсортированный список в файл
    printCentered("Студенты отсортированы по имени.", 100);
}

/*
Отображает статистику: общее количество студентов и средний возраст.
 */
void displayStatistics() {
    if (studentCount == 0) {
        printCentered("Нет записей для расчета статистики.", 100);
        return;
    }
    int totalAge = 0;
    // Суммируем возраст всех студентов
    for (int i = 0; i < studentCount; i++) {
        totalAge += students[i].age;
    }
    // Выводим статистику
    cout << setw(25) << setfill(' ') << "Общее количество студентов: " << studentCount << endl;
    cout << setw(25) << setfill(' ') << "Средний возраст студентов: " << static_cast<float>(totalAge) / studentCount << endl;
}

/*
Главная функция программы.
Отображает меню и обрабатывает выбор пользователя.
 */
int main() {
    SetConsoleOutputCP(1251); // Устанавливаем кодировку Windows-1251 для вывода в консоль, чтобы корректно отображались русские букв
    SetConsoleCP(1251); // Устанавливаем кодировку Windows-1251 для ввода из консоли, чтобы можно было корректно вводить русские имена и названия.

    // Загружаем данные из файла при запуске программы
    loadFromFile();

    int choice; // Переменная для хранения выбора пользователя
    // Главный цикл программы, работает до тех пор, пока пользователь не выберет выход (7)
    do {
        // Отображение меню
        printCentered("-------- Меню Системы Управления Студентами --------", 100);
        printCentered("1. Добавить запись о студенте", 100);
        printCentered("2. Просмотреть все записи о студентах", 100);
        printCentered("3. Удалить запись о студенте", 100);
        printCentered("4. Обновить запись о студенте", 100);
        printCentered("5. Сортировать студентов по имени", 100);
        printCentered("6. Показать статистику", 100);
        printCentered("7. Выход", 100);
        printCentered("--------------------------------------------", 100);
        printCentered("Введите ваш выбор: ", 100);
        cin >> choice;

        // Обработка выбора пользователя
        switch (choice) {
        case 1: insertStudent(); break;
        case 2: viewAllStudents(); break;
        case 3: deleteStudent(); break;
        case 4: updateStudent(); break;
        case 5: sortStudentsByName(); break;
        case 6: displayStatistics(); break;
        case 7: printCentered("Выход из программы.", 50); break;
        default: printCentered("Неверный выбор. Пожалуйста, попробуйте снова.", 50);
        }
    } while (choice != 7);

    return 0;
}
