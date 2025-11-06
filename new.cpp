#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Address {
    string city;
    string state;
    int pin;
};
struct Student {
    int roll;
    string name;
    int age;
    Address addr;
    int marks[6];
    int subjects;
};
void enterStudent(Student &s) {
    cout << "\nEnter Roll Number: ";
    cin >> s.roll;
    cin.ignore();

    cout << "Enter Name: ";
    getline(cin, s.name);

    cout << "Enter Age: ";
    cin >> s.age;

    cin.ignore();
    cout << "Enter City: ";
    getline(cin, s.addr.city);

    cout << "Enter State: ";
    getline(cin, s.addr.state);

    cout << "Enter Pin Code: ";
    cin >> s.addr.pin;

    cout << "How many subjects? (max 6): ";
    cin >> s.subjects;
    if (s.subjects > 6)
        s.subjects = 6;

    for (int i = 0; i < s.subjects; i++) {
        cout << "Enter marks for subject " << i + 1 << ": ";
        cin >> s.marks[i];
    }
}
void showStudent(Student s) {
    
    cout << "Roll No: " << s.roll << endl;
    cout << "Name: " << s.name << endl;
    cout << "Age: " << s.age << endl;
    cout << "City: " << s.addr.city << endl;
    cout << "State: " << s.addr.state << endl;
    cout << "Pin: " << s.addr.pin << endl;

    int total = 0;
    cout << "Marks: ";
    for (int i = 0; i < s.subjects; i++) {
        cout << s.marks[i] << " ";
        total += s.marks[i];
    }
    float avg = (float)total / s.subjects;
    cout << "\nTotal Marks: " << total;
    cout << "\nAverage Marks: " << avg << endl;
    
}


void saveData(Student st[], int n) {
    ofstream file("students.txt");
    if (!file) {
        cout << "Error opening file for saving!" << endl;
        return;
    }

    for (int i = 0; i < n; i++) {
        file << st[i].roll << " " << st[i].name << " " << st[i].age << " "
             << st[i].addr.city << " " << st[i].addr.state << " " << st[i].addr.pin << " "
             << st[i].subjects << " ";
        for (int j = 0; j < st[i].subjects; j++) {
            file << st[i].marks[j] << " ";
        }
        file << endl;
    }

    file.close();
    cout << "\nData saved to file successfully.\n";
}

int loadData(Student st[], int max) {
    ifstream file("students.txt");
    if (!file) {
        cout << "No file found or error reading file!" << endl;
        return 0;
    }

    int count = 0;
    while (file >> st[count].roll) {
        file >> st[count].name >> st[count].age
             >> st[count].addr.city >> st[count].addr.state >> st[count].addr.pin
             >> st[count].subjects;
        for (int i = 0; i < st[count].subjects; i++) {
            file >> st[count].marks[i];
        }
        count++;
        if (count >= max)
            break;
    }
    file.close();
    cout << "\nData loaded from file successfully.\n";
    return count;
}


void showTopper(Student st[], int n) {
    if (n == 0) {
        cout << "No student data available!" << endl;
        return;
    }

    int topIndex = 0;
    int topMarks = 0;

    for (int i = 0; i < n; i++) {
        int total = 0;
        for (int j = 0; j < st[i].subjects; j++) {
            total += st[i].marks[j];
        }
        if (total > topMarks) {
            topMarks = total;
            topIndex = i;
        }
    }

    cout << "\nTop Scorer:\n";
    showStudent(st[topIndex]);
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
                    cout << "Maximum student limit reached!" << endl;
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
                cout << "Invalid choice! Please try again.\n";
        }

    } while (choice != 0);

    return 0;
}
