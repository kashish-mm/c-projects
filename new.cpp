#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Person {
public:
    string name;
    int age;


    Person() {
     
    }

    Person(string n, int a) {
        name = n;
        age = a;
    }

    virtual void showInfo() {
        cout << "Name: " << name << "\nAge: " << age << endl;
    }

    virtual ~Person() {

    }
};

class Student : public Person {
public:
    int roll;
    int m1, m2, m3;

 
    Student() {
        roll = 0;
        m1 = m2 = m3 = 0;
    }

   
    Student(int r, string n, int a, int x, int y, int z)
        : Person(n, a)   
    {
        roll = r;
        m1 = x; 
        m2 = y;
        m3 = z;
    }

   
    void enterStudent() {
        cout << "\nEnter Roll Number: ";
        cin >> roll;
        cin.ignore();

        cout << "Enter Name: ";
        getline(cin, name);

        cout << "Enter Age: ";
        cin >> age;

        cout << "Enter marks of 3 subjects:\n";
        cout << "Subject 1: ";
        cin >> m1;
        cout << "Subject 2: ";
        cin >> m2;
        cout << "Subject 3: ";
        cin >> m3;
    }

  
    void showInfo() override {
        cout << "\nRoll: " << roll << endl;
        Person::showInfo();
        cout << "Marks: " << m1 << ", " << m2 << ", " << m3 << endl;

        int total = m1 + m2 + m3;
        float avg = total / 3.0;

        cout << "Total Marks: " << total << endl;
        cout << "Average: " << avg << endl;
    }


    ~Student() {
     
    }
};
class FileManager {
public:

    void save(Student s[], int count) {
        ofstream file("students.txt");
        if (!file) {
            throw runtime_error("Error opening file for writing!");
        }

        for (int i = 0; i < count; i++) {
            file << s[i].roll << " " << s[i].name << " " << s[i].age << " "
                 << s[i].m1 << " " << s[i].m2 << " " << s[i].m3 << endl;
        }

        file.close();
        cout << "\n✔ Data saved successfully!\n";
    }

    int load(Student s[], int max) {
        ifstream file("students.txt");
        if (!file) {
            throw runtime_error("File not found!");
        }

        int count = 0;
        while (file >> s[count].roll >> s[count].name >> s[count].age 
                    >> s[count].m1 >> s[count].m2 >> s[count].m3) 
        {
            count++;
            if (count >= max)
                break;
        }

        file.close();
        cout << "\n✔ Data loaded successfully!\n";
        return count;
    }
};
void showTopper(Student s[], int n) {
    if (n == 0) {
        cout << "No student records available.\n";
        return;
    }

    int topIndex = 0;
    int topTotal = s[0].m1 + s[0].m2 + s[0].m3;

    for (int i = 1; i < n; i++) {
        int total = s[i].m1 + s[i].m2 + s[i].m3;
        if (total > topTotal) {
            topTotal = total;
            topIndex = i;
        }
    }

    cout << "\n🎉 Topper Student:\n";
    s[topIndex].showInfo();
}
int main() {
    Student s[50];
    int count = 0;
    FileManager fm;

    int choice;

    do {
        cout << "\n===== STUDENT MANAGEMENT SYSTEM =====\n";
        cout << "1. Add Student\n";
        cout << "2. Show All Students\n";
        cout << "3. Save to File\n";
        cout << "4. Load from File\n";
        cout << "5. Show Topper\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            if (count < 50) {
                s[count].enterStudent();
                count++;
            } else {
                cout << "Maximum limit reached!\n";
            }
            break;

        case 2:
            for (int i = 0; i < count; i++) {
                s[i].showInfo();
            }
            break;

        case 3:
            try {
                fm.save(s, count);
            }
            catch (exception &e) {
                cout << "❌ Exception: " << e.what() << endl;
            }
            break;

        case 4:
            try {
                count = fm.load(s, 50);
            }
            catch (exception &e) {
                cout << "❌ Exception: " << e.what() << endl;
            }
            break;

        case 5:
            showTopper(s, count);
            break;

        case 0:
            cout << "Exiting program...\n";
            break;

        default:
            cout << "Invalid choice!\n";
        }

    } while (choice != 0);

    return 0;
}

