#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <limits>
#include <iomanip>

using namespace std;

struct Address {
    string city;
    string state;
    int pin;
};

class User {
public:
    virtual void displayInfo() const = 0;
    virtual ~User() = default;
};

class Person : public User {
protected:
    string name;
    int age;
    const string collegeName;
    Address addr;
public:
    Person(const string &name = "Unknown", int age = 0, const string &college = "ABC College", Address address = {"Unknown", "Unknown", 0})
        : name(name), age(age), collegeName(college), addr(address) {}
    virtual void displayInfo() const override {
        cout << "Name: " << name << ", Age: " << age << ", College: " << collegeName << "\n";
    }
    virtual ~Person() = default;
};

class Exam {
protected:
    vector<int> marks;
public:
    Exam() = default;
    void setMarks(const vector<int> &m) { marks = m; }
    const vector<int>& getMarks() const { return marks; }
};

class Student : virtual public Person {
private:
    int roll;
    vector<int> marks;
    const int maxSubjects = 6;
public:
    static int studentCount;
    Student() : Person(), roll(-1), marks() { ++studentCount; }
    Student(const string &name, int age, int rollNumber, const vector<int> &marksVec, const string &college = "ABC College", Address address = {"Unknown", "Unknown", 0})
        : Person(name, age, college, address), roll(rollNumber), marks(marksVec) { ++studentCount; }
    Student(const Student &other)
        : Person(other.name, other.age, other.collegeName, other.addr), roll(other.roll), marks(other.marks) { ++studentCount; }
    virtual ~Student() { --studentCount; }
    void setRoll(int r) { this->roll = r; }
    int getRoll() const { return roll; }
    const vector<int>& getMarks() const { return marks; }
    void setMarks(const vector<int> &m) {
        if ((int)m.size() > maxSubjects) throw runtime_error("Too many subjects for this student");
        marks = m;
    }
    int totalMarks() const {
        int s = 0;
        for (int m : marks) s += m;
        return s;
    }
    double average() const {
        if (marks.empty()) return 0.0;
        return double(totalMarks()) / marks.size();
    }
    inline void quickPrint() const { cout << name << " (Roll: " << roll << ") -> Avg: " << average() << "\n"; }
    virtual void displayInfo() const override {
        cout << "=== Student Info ===\n";
        cout << "Name: " << name << "\nRoll: " << roll << "\nAge: " << age
             << "\nCollege: " << collegeName << "\nAddress: " << addr.city << ", " << addr.state << " - " << addr.pin << "\n";
        cout << "Marks: ";
        for (int m : marks) cout << m << " ";
        cout << "\nTotal: " << totalMarks() << ", Average: " << average() << "\n";
    }
    bool operator>(const Student &rhs) const { return this->totalMarks() > rhs.totalMarks(); }
    bool operator==(const Student &rhs) const { return this->roll == rhs.roll; }
    friend int compareTotalMarks(const Student &a, const Student &b);
    string serialize() const {
        ostringstream oss;
        oss << roll << '|' << name << '|' << age << '|' << addr.city << '|' << addr.state << '|' << addr.pin << '|';
        for (size_t i = 0; i < marks.size(); ++i) {
            if (i) oss << ';';
            oss << marks[i];
        }
        return oss.str();
    }
    static Student deserialize(const string &line) {
        stringstream ss(line);
        string token;
        vector<string> parts;
        while (getline(ss, token, '|')) parts.push_back(token);
        if (parts.size() < 7) throw runtime_error("Bad record");
        int roll = stoi(parts[0]);
        string name = parts[1];
        int age = stoi(parts[2]);
        Address a{parts[3], parts[4], stoi(parts[5])};
        vector<int> marksVec;
        stringstream ms(parts[6]);
        string mtoken;
        while (getline(ms, mtoken, ';')) {
            if (!mtoken.empty()) marksVec.push_back(stoi(mtoken));
        }
        return Student(name, age, roll, marksVec, "ABC College", a);
    }
};
int Student::studentCount = 0;
int compareTotalMarks(const Student &a, const Student &b) { return (a.totalMarks() - b.totalMarks()); }

class Result : public Student, public Exam {
public:
    Result() = default;
    Result(const Student &s, const vector<int> &examMarks) : Student(s) { Exam::setMarks(examMarks); }
    void displayResult() const {
        cout << "--- Result ---\n";
        displayInfo();
        cout << "Total Marks: " << totalMarks() << " Average: " << average() << "\n";
    }
    using Student::setMarks;  // Make it clear which setMarks to use by default
};

template<typename T>
void displayGeneric(const T &item) { item.displayInfo(); }

template<typename T>
const T& myMax(const T& a, const T& b) { return (a > b) ? a : b; }

class StudentManager {
private:
    Student **arr;
    int capacity;
    int count;
    void expand() {
        int newcap = capacity * 2;
        Student **temp = new Student*[newcap];
        for (int i = 0; i < count; ++i) temp[i] = arr[i];
        for (int i = count; i < newcap; ++i) temp[i] = nullptr;
        delete[] arr;
        arr = temp;
        capacity = newcap;
    }
public:
    StudentManager(int cap = 4) : capacity(cap), count(0) {
        arr = new Student*[capacity];
        for (int i = 0; i < capacity; ++i) arr[i] = nullptr;
    }
    ~StudentManager() {
        for (int i = 0; i < count; ++i) delete arr[i];
        delete[] arr;
    }
    void addStudent(const Student &s) {
        // Check for duplicate roll number
        for (int i = 0; i < count; ++i) {
            if (arr[i]->getRoll() == s.getRoll()) {
                throw runtime_error("Student with this roll number already exists");
            }
        }
        if (count == capacity) expand();
        arr[count++] = new Student(s);
    }
    void addStudent(const string &name, int age, int roll, const vector<int> &marks, const string &college = "ABC College", Address addr = {"Unknown", "Unknown", 0}) {
        // First check for duplicate roll number
        for (int i = 0; i < count; ++i) {
            if (arr[i]->getRoll() == roll) {
                throw runtime_error("Student with this roll number already exists");
            }
        }
        Student *s = new Student(name, age, roll, marks, college, addr);
        try {
            addStudent(*s);
        } catch (...) {
            delete s;
            throw;
        }
        delete s;
    }
    bool removeStudent(int roll) {
        for (int i = 0; i < count; ++i) {
            if (arr[i]->getRoll() == roll) {
                delete arr[i];
                for (int j = i; j < count - 1; ++j) arr[j] = arr[j + 1];
                arr[count - 1] = nullptr;
                --count;
                return true;
            }
        }
        return false;
    }
    bool updateMarks(int roll, const vector<int> &newMarks) {
        for (int i = 0; i < count; ++i) {
            if (arr[i]->getRoll() == roll) {
                try { arr[i]->setMarks(newMarks); }
                catch (const exception &e) { cerr << "Error updating marks: " << e.what() << endl; return false; }
                return true;
            }
        }
        return false;
    }
    Student* findByRoll(int roll) const {
        for (int i = 0; i < count; ++i) if (arr[i]->getRoll() == roll) return arr[i];
        return nullptr;
    }
    void displayAll() const {
        cout << "Total students: " << count << "\n";
        for (int i = 0; i < count; ++i) {
            displayGeneric(*arr[i]);
            cout << "-------------------------\n";
        }
    }
    void saveToFile(const string &filename) const {
        ofstream ofs(filename);
        if (!ofs) throw runtime_error("Cannot open file for writing: " + filename);
        for (int i = 0; i < count; ++i) ofs << arr[i]->serialize() << "\n";
        ofs.close();
    }
    void loadFromFile(const string &filename) {
        ifstream ifs(filename);
        if (!ifs) throw runtime_error("Cannot open file for reading: " + filename);
        for (int i = 0; i < count; ++i) delete arr[i];
        count = 0;
        string line;
        while (getline(ifs, line)) {
            if (line.empty()) continue;
            Student s = Student::deserialize(line);
            addStudent(s);
        }
        ifs.close();
    }
    Student* topScorer() const {
        if (count == 0) return nullptr;
        int bestIdx = 0;
        for (int i = 1; i < count; ++i) if (*arr[i] > *arr[bestIdx]) bestIdx = i;
        return arr[bestIdx];
    }
    double allStudentsAverage() const {
        if (count == 0) return 0.0;
        double total = 0;
        int subjects = 0;
        for (int i = 0; i < count; ++i) {
            total += arr[i]->average();
            ++subjects;
        }
        return total / subjects;
    }
    int size() const { return count; }
};

inline void printMenu() {
    cout << "\n=== Student Management System Menu ===\n"
         << "1. Add student\n2. Remove student\n3. Update marks\n4. Display all students\n5. Save to file\n6. Load from file\n7. Show top scorer\n8. Show statistics\n9. Demo compare two students\n0. Exit\nChoose option: ";
}

int main() {
    StudentManager mgr(4);
    try {
        Address a1{"Mumbai", "MH", 400001};
        mgr.addStudent("Kapil", 21, 101, {85, 90, 78}, "ABC College", a1);
        mgr.addStudent("Ramesh", 22, 102, {88, 76, 95}, "ABC College", {"Pune", "MH", 411001});
    } catch (const exception &e) { cerr << "Error creating initial students: " << e.what() << "\n"; }

    bool exitFlag = false;
    while (!exitFlag) {
        try {
            printMenu();
            int choice;
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw runtime_error("Invalid input for menu choice.");
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Clear input buffer after menu choice
            
            switch (choice) {
                case 1: {
                    string name; int age, roll; int subCount;
                    Address addr;
                    cout << "Enter name: ";
                    getline(cin, name);
                    cout << "Enter age: "; cin >> age;
                    if (age < 0 || age > 150) throw runtime_error("Invalid age");
                    cout << "Enter roll: "; cin >> roll;
                    if (roll < 0) throw runtime_error("Invalid roll number");
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Enter city: "; getline(cin, addr.city);
                    cout << "Enter state: "; getline(cin, addr.state);
                    cout << "Enter pin: "; cin >> addr.pin;
                    if (addr.pin < 0) throw runtime_error("Invalid PIN code");
                    cout << "How many marks to enter? "; cin >> subCount;
                    if (subCount <= 0 || subCount > 6) throw runtime_error("Invalid number of subjects (1-6)");
                    vector<int> marks(subCount);
                    for (int i = 0; i < subCount; ++i) {
                        cout << "Mark " << i+1 << ": "; 
                        cin >> marks[i];
                        if (marks[i] < 0 || marks[i] > 100) throw runtime_error("Invalid mark entered (0-100)");
                    }
                    mgr.addStudent(name, age, roll, marks, "ABC College", addr);
                    cout << "Added student.\n";
                    break;
                }
                case 2: {
                    cout << "Enter roll to remove: "; int r; cin >> r;
                    if (mgr.removeStudent(r)) cout << "Removed.\n"; else cout << "Not found.\n";
                    break;
                }
                case 3: {
                    cout << "Enter roll to update marks: "; int r; cin >> r;
                    int k; cout << "How many marks? "; cin >> k;
                    if (k <= 0 || k > 6) throw runtime_error("Invalid number of subjects (1-6)");
                    vector<int> m(k);
                    for (int i = 0; i < k; ++i) {
                        cout << "Mark " << i+1 << ": ";
                        cin >> m[i];
                        if (m[i] < 0 || m[i] > 100) throw runtime_error("Invalid mark entered (0-100)");
                    }
                    if (mgr.updateMarks(r, m)) cout << "Updated.\n"; else cout << "Update failed.\n";
                    break;
                }
                case 4:
                    mgr.displayAll();
                    break;
                case 5: {
                    string fname;
                    cout << "Enter filename to save: ";
                    getline(cin, fname);
                    mgr.saveToFile(fname);
                    cout << "Saved to file.\n";
                    break;
                }
                case 6: {
                    string fname;
                    cout << "Enter filename to load: ";
                    getline(cin, fname);
                    mgr.loadFromFile(fname);
                    cout << "Loaded data from file.\n";
                    break;
                }
                case 7: {
                    Student* top = mgr.topScorer();
                    if (top) {
                        cout << "Top scorer:\n";
                        top->displayInfo();
                    } else {
                        cout << "No students.\n";
                    }
                    break;
                }
                case 8: {
                    cout << "Total students (manager): " << mgr.size() << "\n";
                    cout << "Student::studentCount (static): " << Student::studentCount << "\n";
                    cout << "Average across students: " << fixed << setprecision(2) << mgr.allStudentsAverage() << "\n";
                    break;
                }
                case 9: {
                    int a, b;
                    cout << "Enter roll a: ";
                    if (!(cin >> a)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        throw runtime_error("Invalid roll number");
                    }
                    cout << "Enter roll b: ";
                    if (!(cin >> b)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        throw runtime_error("Invalid roll number");
                    }
                    Student* sa = mgr.findByRoll(a);
                    Student* sb = mgr.findByRoll(b);
                    if (!sa || !sb) {
                        cout << "One or both students not found.\n";
                        break;
                    }
                    cout << "Are they same roll? " << ((*sa == *sb) ? "Yes\n" : "No\n");
                    int diff = compareTotalMarks(*sa, *sb);
                    if (diff > 0) {
                        cout << sa->getRoll() << " has more marks by " << diff << "\n";
                    } else if (diff < 0) {
                        cout << sb->getRoll() << " has more marks by " << -diff << "\n";
                    } else {
                        cout << "They have same total marks.\n";
                    }
                    break;
                }
                case 0:
                    exitFlag = true;
                    break;
                default:
                    cout << "Unknown option.\n";
            }
        } catch (const exception &ex) {
            cerr << "Exception: " << ex.what() << "\n";
        }
    }
    cout << "Exiting. Final student count (static): " << Student::studentCount << "\n";
    return 0;
}
