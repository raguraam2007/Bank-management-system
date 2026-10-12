#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cstdio>
using namespace std;

struct Student {
    int rollNo;
    string name;
    string department;
    float marks;
};

const string FILE_NAME = "students.txt";

bool rollExists(int rollNo) {
    ifstream file(FILE_NAME);
    Student s;

    while (file >> s.rollNo) {
        file.ignore(numeric_limits<streamsize>::max(), '\n');
        if (!getline(file, s.name) || !getline(file, s.department) ||
            !(file >> s.marks)) break;
        file.ignore(numeric_limits<streamsize>::max(), '\n');
        if (s.rollNo == rollNo) return true;
    }
    return false;
}

void addStudent() {
    Student s;
    cout << "\nEnter Roll Number: ";
    if (!(cin >> s.rollNo) || s.rollNo <= 0) {
        cout << "Invalid roll number!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    if (rollExists(s.rollNo)) {
        cout << "Roll number already exists!\n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter Name: ";
    getline(cin, s.name);
    cout << "Enter Department: ";
    getline(cin, s.department);
    cout << "Enter Marks (0-100): ";

    if (!(cin >> s.marks) || s.marks < 0 || s.marks > 100) {
        cout << "Marks must be between 0 and 100!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    ofstream file(FILE_NAME, ios::app);
    if (!file) {
        cout << "Unable to open student file!\n";
        return;
    }

    file << s.rollNo << '\n' << s.name << '\n'
         << s.department << '\n' << s.marks << '\n';
    cout << "Student added successfully!\n";
}

void displayStudents() {
    ifstream file(FILE_NAME);
    Student s;
    bool found = false;

    if (!file) {
        cout << "\nNo student records found.\n";
        return;
    }

    cout << "\n========== STUDENT RECORDS ==========\n";
    while (file >> s.rollNo) {
        file.ignore(numeric_limits<streamsize>::max(), '\n');
        if (!getline(file, s.name) || !getline(file, s.department) ||
            !(file >> s.marks)) {
            cout << "Warning: Invalid record in file.\n";
            break;
        }
        file.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "\nRoll Number : " << s.rollNo
             << "\nName        : " << s.name
             << "\nDepartment  : " << s.department
             << "\nMarks       : " << s.marks << "/100\n";
        found = true;
    }

    if (!found) cout << "No student records found.\n";
}

bool saveWithoutRoll(int rollNo, bool update) {
    ifstream file(FILE_NAME);
    if (!file) {
        cout << "No student records found.\n";
        return false;
    }

    ofstream temp("temp.txt");
    if (!temp) {
        cout << "Unable to create temporary file!\n";
        return false;
    }

    Student s;
    bool found = false;
    bool valid = true;

    while (file >> s.rollNo) {
        file.ignore(numeric_limits<streamsize>::max(), '\n');
        if (!getline(file, s.name) || !getline(file, s.department) ||
            !(file >> s.marks)) {
            valid = false;
            break;
        }
        file.ignore(numeric_limits<streamsize>::max(), '\n');

        if (s.rollNo == rollNo) {
            found = true;
            if (!update) continue;

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Enter New Name: ";
            getline(cin, s.name);
            cout << "Enter New Department: ";
            getline(cin, s.department);
            cout << "Enter New Marks (0-100): ";

            if (!(cin >> s.marks) || s.marks < 0 || s.marks > 100) {
                cout << "Invalid marks! Update cancelled.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                valid = false;
                break;
            }
        }

        temp << s.rollNo << '\n' << s.name << '\n'
             << s.department << '\n' << s.marks << '\n';
    }

    file.close();
    temp.close();

    if (!valid) {
        remove("temp.txt");
        return false;
    }

    if (!found) {
        remove("temp.txt");
        cout << "Student not found!\n";
        return false;
    }

    if (remove(FILE_NAME.c_str()) != 0) {
        cout << "Could not replace the student data file.\n";
        remove("temp.txt");
        return false;
    }
    if (rename("temp.txt", FILE_NAME.c_str()) != 0) {
        cout << "Could not save the updated records. Your original file may need restoring.\n";
        return false;
    }

    cout << (update ? "Student updated successfully!\n"
                    : "Student deleted successfully!\n");
    return true;
}

void updateStudent() {
    int rollNo;
    cout << "\nEnter Roll Number to update: ";
    if (!(cin >> rollNo)) {
        cout << "Invalid input!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    saveWithoutRoll(rollNo, true);
}

void deleteStudent() {
    int rollNo;
    cout << "\nEnter Roll Number to delete: ";
    if (!(cin >> rollNo)) {
        cout << "Invalid input!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    saveWithoutRoll(rollNo, false);
}

void searchStudent() {
    int rollNo;
    cout << "\nEnter Roll Number to search: ";
    if (!(cin >> rollNo)) {
        cout << "Invalid input!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    ifstream file(FILE_NAME);
    Student s;
    while (file >> s.rollNo) {
        file.ignore(numeric_limits<streamsize>::max(), '\n');
        if (!getline(file, s.name) || !getline(file, s.department) ||
            !(file >> s.marks)) break;
        file.ignore(numeric_limits<streamsize>::max(), '\n');

        if (s.rollNo == rollNo) {
            cout << "\nStudent Found!\n"
                 << "Roll Number: " << s.rollNo
                 << "\nName: " << s.name
                 << "\nDepartment: " << s.department
                 << "\nMarks: " << s.marks << "/100\n";
            return;
        }
    }
    cout << "Student not found!\n";
}

int main() {
    int choice;
    do {
        cout << "\n====================================\n"
             << "     STUDENT MANAGEMENT SYSTEM\n"
             << "====================================\n"
             << "1. Add Student\n"
             << "2. Display All Students\n"
             << "3. Search Student\n"
             << "4. Update Student\n"
             << "5. Delete Student\n"
             << "6. Exit\n"
             << "====================================\n"
             << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Please enter a valid number!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: cout << "Thank you for using Student Management System!\n"; break;
            default: cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 6);

    return 0;
}
