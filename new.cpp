#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Student {
    int roll;
    string name;
    int age;
    int marks1, marks2, marks3;
};

void enterStudent(Student &s) {
    cout << "\nEnter Roll Number: ";
    cin >> s.roll;
    cin.ignore();

    cout << "Enter Name: ";
    getline(cin, s.name);

    cout << "Enter Age: ";
    cin >> s.age;

    cout << "Enter Marks for 3 Subjects:\n";
    cout << "Subject 1: ";
    cin >> s.marks1;
    cout << "Subject 2: ";
    cin >> s.marks2;
    cout << "Subject 3: ";
    cin >> s.marks3;
}

void showStudent(Student s) {
    cout << "\nRoll No: " << s.roll << endl;
    cout << "Name: " << s.name << endl;
    cout << "Age: " << s.age << endl;
    cout << "Marks: " << s.marks1 << ", " << s.marks2 << ", " << s.marks3 << endl;

    int total = s.marks1 + s.marks2 + s.marks3;
    float avg = total / 3.0;
    cout << "Total Marks: " << total << endl;
    cout << "Average Marks: " << avg << endl;
}

void saveData(Student s[], int count) {
    ofstream file("students.txt");
    if (!file) {
        cout << "Error opening file!" << endl;
        return;
    }

    for (int i = 0; i < count; i++) {
        file << s[i].roll << " " << s[i].name << " " << s[i].age << " "
             << s[i].marks1 << " " << s[i].marks2 << " " << s[i].marks3 << endl;
    }

    file.close();
    cout << "\nData saved successfully to file!\n";
}

int loadData(Student s[], int max) {
    ifstream file("students.txt");
    if (!file) {
        cout << "No file found or error reading file!\n";
        return 0;
    }

    int count = 0;
    while (file >> s[count].roll >> s[count].name >> s[count].age
           >> s[count].marks1 >> s[count].marks2 >> s[count].marks3) {
        count++;
        if (count >= max)
            break;
    }

    file.close();
    cout << "\nData loaded successfully from file!\n";
    return count;
}

void showTopper(Student s[], int n) {
    if (n == 0) {
        cout << "No student data available!" << endl;
        return;
    }

    int topIndex = 0;
    int topMarks = s[0].marks1 + s[0].marks2 + s[0].marks3;

    for (int i = 1; i < n; i++) {
        int total = s[i].marks1 + s[i].marks2 + s[i].marks3;
        if (total > topMarks) {
            topMarks = total;
            topIndex = i;
        }
    }

    cout << "\nTop Scorer:\n";
    showStudent(s[topIndex]);
}

int main() {
    Student s[50];
    int count = 0;
    int choice;

    do {
        cout << "\n===== STUDENT MANAGEMENT SYSTEM =====\n";
        cout << "1. Add New Student\n";
        cout << "2. Show All Students\n";
        cout << "3. Save to File\n";
        cout << "4. Load from File\n";
        cout << "5. Show Topper\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                if (count < 50) {
                    enterStudent(s[count]);
                    count++;
                } else {
                    cout << "Maximum student limit reached!\n";
                }
                break;

            case 2:
                for (int i = 0; i < count; i++) {
                    showStudent(s[i]);
                }
                break;

            case 3:
                saveData(s, count);
                break;

            case 4:
                count = loadData(s, 50);
                break;

            case 5:
                showTopper(s, count);
                break;

            case 0:
                cout << "Exiting program. Thank you!\n";
                break;

            default:
                cout << "Invalid choice! Try again.\n";
        }

    } while (choice != 0);

    return 0;
}
