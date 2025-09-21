#include <iostream>  // ��� ������ � �������� �����-������ (cin, cout, cerr)
#include <cstring>   // ��� ������ � C-�������� (strcpy, strcmp � �.�.)
#include <iomanip>   // ��� ����������� � ������� (setw, setfill)
#include <fstream>   // ��� ������ � ��������� �������� (ofstream, ifstream)
#include <Windows.h> // ��� ������ � �������� �� ������� �����

// ���������� ����������� ������������ ����, ����� �� ������ std:: ����� ������ ��������
using namespace std;

// --- ���������� ��������� ---
const int MAX_STUDENTS = 100;                 // ������������ ���������� ��������� � ����
const int MAX_NAME_LENGTH = 50;               // ������������ ����� ����� ��������
const int MAX_COURSE_LENGTH = 50;             // ������������ ����� �������� �����

// --- ��������� ��� �������� ������ � �������� ---
struct Student {
    int id;                         // ���������� ������������� ��������
    char name[MAX_NAME_LENGTH];     // ��� ��������
    int age;                        // ������� ��������
    char course[MAX_COURSE_LENGTH]; // �������� �����, �� ������� ��������� �������
};

// --- ���������� ���������� ---
Student students[MAX_STUDENTS]; // ������ ��� �������� ���� ������� � ���������
int studentCount = 0;           // ������� ���������� ��������� � ����

// --- ������ � ������ ---
const char* FILE_NAME = "students.dat"; // ��� ����� ��� ���������� � �������� ������ (��������)

/*
��������� ��� ������� ������ � ��������� � �������� ����.
������� �������������� ���� ������ ��� ��� ������.
 */
void saveToFile() {
    // ��������� ���� ��� ������ � �������� ������ (ios::binary) � � ��������� ����������� ����������� (ios::trunc)
    ofstream out(FILE_NAME, ios::binary | ios::trunc);
    // ���������, ������� �� �������� ����
    if (!out) {
        cerr << "������ �������� ����� ��� ������." << endl;
        return; // ������� �� �������, ���� ���� �� ��������
    }
    // ���������� ������� ���������� ���������
    out.write(reinterpret_cast<char*>(&studentCount), sizeof(studentCount));
    // ����� ���������� ���� ������ ���������
    out.write(reinterpret_cast<char*>(students), sizeof(Student) * studentCount);
    // ��������� ���� ����� ���������� ������
    out.close();
}

/*
��������� ������ � ��������� �� ��������� �����.
���������� ��� ������� ��������� ��� �������������� ������.
 */
void loadFromFile() {
    // ��������� ���� ��� ������ � �������� ������
    ifstream in(FILE_NAME, ios::binary);
    // ���������, �������� �� ����. ���� ���, ��� ����� ��������, ��� ��������� ����������� �������.
    if (!in) {
        // ����� ����� �� ���� ��� ������ ������� � ��� ���������
        return; // ������ �������, ���� ������ ����� ������
    }
    // ��������� ���������� ���������
    in.read(reinterpret_cast<char*>(&studentCount), sizeof(studentCount));
    // ��������� ������ ��������� � ������
    in.read(reinterpret_cast<char*>(students), sizeof(Student) * studentCount);
    // ��������� ����
    in.close();
}

/*
������� ����� �� ������ �������.
 * @param text ����� ��� ������.
 * @param width ����� ������ ���� ��� �������������.
 */
void printCentered(const string& text, int width) {
    // ������������ ���������� �������� �����
    int padding = (width - text.length()) / 2;
    // �������� �������
    for (int i = 0; i < padding; ++i) {
        cout << " ";
    }
    // �������� ��� �����
    cout << text << endl;
}

/*
���������, �������� �� ���������� ID ����������.
 * @param id ������������� ��� ��������.
 * @return true, ���� ID ��������, false - � ��������� ������.
 */
bool isUniqueID(int id) {
    // �������� �� ���� ������������ ���������
    for (int i = 0; i < studentCount; i++) {
        // ���� ������� �������� � ����� �� ID, ���������� false
        if (students[i].id == id) {
            return false;
        }
    }
    // ���� ���� ���������� � ���������� �� �������, ID ��������
    return true;
}

/*
��������� ����� ������ � �������� � ���� ������.
 */
void insertStudent() {
    // ���������, �� ��������� �� ����� ���������
    if (studentCount >= MAX_STUDENTS) {
        printCentered("������ ��������� �����.", 100);
        return;
    }

    int id;
    printCentered("������� ID ��������: ", 100);
    cin >> id;
    // ��������� ������������ ID. ���� ID ��� ����������, ������ ������ ������.
    while (!isUniqueID(id)) {
        printCentered("����� ID ��� ����������. ������� ���������� ID: ", 100);
        cin >> id;
    }
    students[studentCount].id = id;

    // ������� ����� ����� ����� ������� ������
    cin.ignore();

    printCentered("������� ��� ��������: ", 100);
    // ��������� ������ � ������
    cin.getline(students[studentCount].name, MAX_NAME_LENGTH);

    printCentered("������� ������� ��������: ", 100);
    cin >> students[studentCount].age;
    // ��������� ������������ ����� ��������
    while (cin.fail() || students[studentCount].age <= 0 || students[studentCount].age > 100) {
        cin.clear(); // ���������� ����� ������ ������ �����
        cin.ignore(1000, '\n'); // ������� ����� �����
        printCentered("�������� �������. ������� ���������� ������� (1-100): ", 100);
        cin >> students[studentCount].age;
    }
    // ������� ����� ����� ����� ������ �����
    cin.ignore();

    printCentered("������� �������� �����: ", 100);
    cin.getline(students[studentCount].course, MAX_COURSE_LENGTH);

    // ����������� ������� ���������
    studentCount++;
    // ��������� ��������� � ����
    saveToFile();
    printCentered("������ � �������� ������� ���������.", 100);
}


/*
���������� ������ ���� ��������� � ���� ������.
 */
void viewAllStudents() {
    // ���������, ���� �� ������ ��� �����������
    if (studentCount == 0) {
        printCentered("������ � ��������� �� �������.", 100);
        return;
    }
    // ������� ��������� �������
    printCentered("ID\t���\t\t�������\t����", 100);
    printCentered("-------------------------------------------", 100);
    // �������� �� ���� ��������� � ������� �� ������
    for (int i = 0; i < studentCount; i++) {
        cout << setw(25) << setfill(' ') << students[i].id << "\t"
            << setw(25) << setfill(' ') << students[i].name << "\t\t"
            << setw(25) << setfill(' ') << students[i].age << "\t"
            << setw(25) << setfill(' ') << students[i].course << endl;
    }
}

/*
������� ������ � �������� �� ��� ID.
 */
void deleteStudent() {
    int id;
    printCentered("������� ID �������� ��� ��������: ", 100);
    cin >> id;
    // ���� �������� � ��������� ID
    for (int i = 0; i < studentCount; i++) {
        if (students[i].id == id) {
            // ����� ������� ������, �������� ��� ����������� �������� ������� ����� �� ���� �������
            for (int j = i; j < studentCount - 1; j++) {
                students[j] = students[j + 1];
            }
            // ��������� ����� ���������� ���������
            studentCount--;
            saveToFile(); // ��������� ��������� � ����
            printCentered("������ � �������� ������� �������.", 100);
            return; // ������� �� �������
        }
    }
    // ���� ���� ����������, � ������� �� ������
    printCentered("������� � ����� ID �� ������.", 100);
}

/*
��������� ������ ������������� �������� �� ��� ID.
 */
void updateStudent() {
    int id;
    printCentered("������� ID �������� ��� ����������: ", 100);
    cin >> id;
    // ���� �������� ��� ����������
    for (int i = 0; i < studentCount; i++) {
        if (students[i].id == id) {
            // ������� ����� �����
            cin.ignore();

            // ����������� ����� ������
            printCentered("������� ����� ��� ��������: ", 100);
            cin.getline(students[i].name, MAX_NAME_LENGTH);

            printCentered("������� ����� ������� ��������: ", 100);
            cin >> students[i].age;
            // ��������� ������������ ����� ��������
            while (cin.fail() || students[i].age <= 0 || students[i].age > 100) {
                cin.clear();
                cin.ignore(1000, '\n');
                printCentered("�������� �������. ������� ���������� ������� (1-100): ", 100);
                cin >> students[i].age;
            }
            cin.ignore();

            printCentered("������� ����� ���� ��������: ", 100);
            cin.getline(students[i].course, MAX_COURSE_LENGTH);

            saveToFile(); // ��������� ����������� ������ � ����
            printCentered("������ � �������� ������� ���������.", 100);
            return;
        }
    }
    // ���� ������� �� ������
    printCentered("������� � ����� ID �� ������.", 100);
}

/*
��������� ������ ��������� �� ����� � ���������� �������.
������������ �������� ����������� ����������.
 */
void sortStudentsByName() {
    // ���������� ��������� ����������� ����������
    for (int i = 0; i < studentCount - 1; i++) {
        for (int j = 0; j < studentCount - i - 1; j++) {
            // ���������� ����� ���� �������� ���������
            if (strcmp(students[j].name, students[j + 1].name) > 0) {
                // ���� ������� ��������, ������ �� �������
                Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }
    saveToFile(); // ��������� ��������������� ������ � ����
    printCentered("�������� ������������� �� �����.", 100);
}

/*
���������� ����������: ����� ���������� ��������� � ������� �������.
 */
void displayStatistics() {
    if (studentCount == 0) {
        printCentered("��� ������� ��� ������� ����������.", 100);
        return;
    }
    int totalAge = 0;
    // ��������� ������� ���� ���������
    for (int i = 0; i < studentCount; i++) {
        totalAge += students[i].age;
    }
    // ������� ����������
    cout << setw(25) << setfill(' ') << "����� ���������� ���������: " << studentCount << endl;
    cout << setw(25) << setfill(' ') << "������� ������� ���������: " << static_cast<float>(totalAge) / studentCount << endl;
}

/*
������� ������� ���������.
���������� ���� � ������������ ����� ������������.
 */
int main() {
    SetConsoleOutputCP(1251); // ������������� ��������� Windows-1251 ��� ������ � �������, ����� ��������� ������������ ������� ����
    SetConsoleCP(1251); // ������������� ��������� Windows-1251 ��� ����� �� �������, ����� ����� ���� ��������� ������� ������� ����� � ��������.

    // ��������� ������ �� ����� ��� ������� ���������
    loadFromFile();

    int choice; // ���������� ��� �������� ������ ������������
    // ������� ���� ���������, �������� �� ��� ���, ���� ������������ �� ������� ����� (7)
    do {
        // ����������� ����
        printCentered("-------- ���� ������� ���������� ���������� --------", 100);
        printCentered("1. �������� ������ � ��������", 100);
        printCentered("2. ����������� ��� ������ � ���������", 100);
        printCentered("3. ������� ������ � ��������", 100);
        printCentered("4. �������� ������ � ��������", 100);
        printCentered("5. ����������� ��������� �� �����", 100);
        printCentered("6. �������� ����������", 100);
        printCentered("7. �����", 100);
        printCentered("--------------------------------------------", 100);
        printCentered("������� ��� �����: ", 100);
        cin >> choice;

        // ��������� ������ ������������
        switch (choice) {
        case 1: insertStudent(); break;
        case 2: viewAllStudents(); break;
        case 3: deleteStudent(); break;
        case 4: updateStudent(); break;
        case 5: sortStudentsByName(); break;
        case 6: displayStatistics(); break;
        case 7: printCentered("����� �� ���������.", 50); break;
        default: printCentered("�������� �����. ����������, ���������� �����.", 50);
        }
    } while (choice != 7);

    return 0;
}
