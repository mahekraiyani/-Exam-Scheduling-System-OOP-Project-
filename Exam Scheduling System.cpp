#include <bits/stdc++.h>
using namespace std;

string toLower(string s)
{
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

class Classroom
{
    string classNo;
    int capacity;

public:
    Classroom(string n, int c)
    {
        classNo = n;
        capacity = c;
    }
    void displayInfo()
    {
        cout << "Classroom : " << classNo << " | Capacity : " << capacity;
    }
    string getClass()
    {
        return classNo;
    }
    int getCapacity()
    {
        return capacity;
    }
};

class Exam
{
protected:
    string name, date;
    vector<Classroom *> assignclass;
public:
    Exam(string n, string d)
    {
        name = n;
        date = d;
    }
    virtual void displayexam()
    {
        cout << " Exam subject : " << name << " | Date : " << date;
        if (!assignclass.empty())
        {
            cout << " | Classrooms : ";
            for (auto r : assignclass)
            {
                cout << r->getClass() << " ";
            }
            cout << endl;
        }
    }
    virtual void genSchedule() = 0;
    string getname()
    {
        return name;
    }
    string getdate()
    {
        return date;
    }
    void addClassroom(Classroom *c)
    {
        assignclass.push_back(c);
    }
    vector<Classroom *> getrooms()
    {
        return assignclass;
    }
};

class Theory : public Exam
{
    int duration, maxMarks;

public:
    Theory(string n, string d, int dur, int mm)
        : Exam(n, d), duration(dur), maxMarks(mm) {}
    void genSchedule() override
    {
        cout << "Theory Exam Schedule : \n";
        displayexam();
        cout << " | Duration : " << duration << " mins";
        cout << " | Maximum marks : " << maxMarks << endl;
    }
};

class Practical : public Exam
{
    int vivamarks;

public:
    Practical(string n, string d, int v)
        : Exam(n, d), vivamarks(v) {}
    void genSchedule() override
    {
        cout << "Practical Exam Schedule : \n";
        displayexam();
        cout << " | Vivamarks : " << vivamarks << endl;
    }
};

class Student
{
    string rollNo, name;
public:
    vector<Exam *> exams;
    Student(string r, string n)
    {
        rollNo = r;
        name = n;
    }
    void addexam(Exam *e)
    {
        exams.push_back(e);
    }
    void Timetable()
    {
        cout << "\n===== Timetable for " << name << " (" << rollNo << ") =====\n";
        if (exams.empty())
        {
            cout << "No exams assigned.\n";
        }
        else
        {
            cout << left << setw(20) << "Exam"
                 << setw(15) << "Date"
                 << "Classrooms\n";
            for (auto e : exams)
            {
                cout << left << setw(20) << e->getname()
                     << setw(15) << e->getdate();
                auto rooms = e->getrooms();
                for (size_t i = 0; i < rooms.size(); i++)
                {
                    cout << rooms[i]->getClass();
                    if (i != rooms.size() - 1)
                        cout << ", ";
                }
                cout << "\n";
            }
        }
    }
    string getrollNo()
    {
        return rollNo;
    }
    string getName()
    {
        return name;
    }
    vector<Exam *> getExams()
    {
        return exams;
    }
};

class Schedule
{
    vector<Exam *> exams;
    vector<Student *> students;
    vector<Classroom *> rooms;
    map<string, vector<pair<Student *, string>>> examAssignments;

public:
    Schedule()
    {
        exams.clear();
        rooms.clear();
        students.clear();
        examAssignments.clear();
    }
    void addExam(Exam *e)
    {
        exams.push_back(e);
    }
    void addStudents(Student *stud)
    {
        students.push_back(stud);
    }
    void addrooms(Classroom *c)
    {
        rooms.push_back(c);
    }
    void assignclass()
    {
        if (rooms.empty())
        {
            cout << "No classrooms available!!" << endl;
        }
        else
        {
            for (auto e : exams)
            {
                for (auto r : rooms)
                {
                    e->addClassroom(r);
                    cout << "Exam " << e->getname() << " assigned to class " << r->getClass() << endl;
                }
            }
        }
    }
    void finalSchedule()
    {
        if (exams.empty())
        {
            cout << "No exams added yet!\n";
            return;
        }
        sort(exams.begin(), exams.end(), [](Exam *a, Exam *b)
             { return a->getdate() < b->getdate(); });
        cout << "\n===== Final Exam Schedule =====\n";
        for (auto e : exams)
        {
            cout << "\n--------------------------------\n";
            e->genSchedule();
        }
        cout << "--------------------------------\n";
    }
    void finalExams()
    {
        for (auto e : exams)
        {
            cout << e->getname() << " | " << e->getdate() << endl;
        }
    }
    void finalStudents()
    {
        for (auto s : students)
        {
            cout << s->getrollNo() << " | " << s->getName() << endl;
        }
    }
    bool hasConflict(Student *stud, string date)
    {
        for (auto e : stud->exams)
        {
            if (e->getdate() == date)
            {
                return true;
            }
        }
        return false;
    }
    void assignClassroomToExam(string ename, string rid)
    {
        Exam *ex = nullptr;
        for (auto e : exams)
        {
            if (toLower(e->getname()) == ename)
            { // matching exam by name
                ex = e;
                break;
            }
        }
        if (!ex)
        {
            cout << "Exam not found!\n";
            return;
        }

        Classroom *room = nullptr;
        for (auto r : rooms)
        {
            if (toLower(r->getClass()) == rid)
            { // matching classroom by id
                room = r;
                break;
            }
        }
        if (!room)
        {
            cout << "Classroom not found!\n";
            return;
        }
        for (auto other : exams)
        {
            if (other != ex)
            {
                for (auto r : other->getrooms())
                {
                    if (r == room && other->getdate() == ex->getdate())
                    {
                        cout << "Conflict: Classroom " << rid
                             << " already assigned to exam "
                             << other->getname()
                             << " on date " << ex->getdate() << endl;
                        return;
                    }
                }
            }
        }

        ex->addClassroom(room);
        cout << "Assigned classroom " << rid << " to exam " << ename << endl;
    }

    void assignExamtoStud(string sid, string ename)
    {
        Student *stud = nullptr;
        for (auto s : students)
        {
            if (toLower(s->getrollNo()) == toLower(sid))
            {
                stud = s;
                break;
            }
        }
        if (!stud)
        {
            cout << "Student not found!!\n";
            return;
        }

        Exam *ex = nullptr;
        for (auto e : exams)
        {
            if (toLower(e->getname()) == toLower(ename))
            {
                ex = e;
                break;
            }
        }
        if (!ex)
        {
            cout << "Exam subject not found!!\n";
            return;
        }

        // Check date conflict
        for (auto e : stud->getExams())
        {
            if (e->getdate() == ex->getdate())
            {
                cout << "Student " << sid << " already has an exam on " << ex->getdate() << "\n";
                return;
            }
        }

        // Count students per classroom
        bool seatAssigned = false;
        for (auto room : ex->getrooms())
        {
            int countInRoom = 0;
            for (auto &a : examAssignments[ename])
            {
                if (a.second == room->getClass())
                    countInRoom++;
            }

            if (countInRoom < room->getCapacity())
            {
                stud->addexam(ex);
                examAssignments[ename].push_back({stud, room->getClass()});
                cout << "Exam " << ename << " assigned to student " << sid
                     << " in classroom " << room->getClass() << "\n";
                seatAssigned = true;
                break;
            }
        }

        if (!seatAssigned)
        {
            cout << "No available seats for exam " << ename << "!!\n";
        }
    }
    void viewStudentTimetables()
    {
        if (students.empty())
        {
            cout << "No students available.\n";
            return;
        }
        for (auto s : students)
        {
            s->Timetable();
        }
    }
    void showExamseating()
    {
        if (examAssignments.empty())
        {
            cout << "No students assigned to exams yet.\n";
            return;
        }

        for (auto &entry : examAssignments)
        {
            string ename = entry.first;
            vector<pair<Student *, string>> assigned = entry.second;
            cout << "\n===== Exam: " << ename << " =====\n";
            if (assigned.empty())
            {
                cout << "No students assigned.\n";
                continue;
            }
            map<string, vector<Student *>> roomwise;
            for (auto &a : assigned)
                roomwise[a.second].push_back(a.first);

            for (auto &r : roomwise)
            {
                cout << "Classroom " << r.first << ":\n";
                int cnt = 1;
                for (auto stud : r.second)
                {
                    cout << "  " << setw(3) << cnt++ << ". "
                         << setw(10) << stud->getrollNo()
                         << " - " << stud->getName() << "\n";
                }
            }
        }
        cout << "=======================================\n";
    }
};

int main()
{
    Schedule sched;
    int choice;

    while (true)
    {
        cout << "\n================= Exam Scheduler Menu =================\n";
        cout << "1. Add Classroom\n";
        cout << "2. Add Theory Exam\n";
        cout << "3. Add Practical Exam\n";
        cout << "4. Add Student\n";
        cout << "5. Assign Classrooms to Exam\n";
        cout << "6. Generate Exam Schedule\n";
        cout << "7. View Student Timetables\n";
        cout << "8. Assign Exams to Student\n";
        cout << "9. Show Exam Seating Report\n";
        cout << "10. Exit\n";
        cout << "==========================================================\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            string rid;
            int cap;
            cout << "Enter Room ID: ";
            cin >> rid;
            cout << "Enter Capacity: ";
            cin >> cap;
            sched.addrooms(new Classroom(rid, cap));
            break;
        }
        case 2:
        {
            string eid, name, date;
            int dur, marks;
            cout << "Enter Exam Name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter Date: ";
            cin >> date;
            cout << "Enter Duration: ";
            cin >> dur;
            cout << "Enter Max Marks: ";
            cin >> marks;
            sched.addExam(new Theory(name, date, dur, marks));
            break;
        }
        case 3:
        {
            string name, date;
            int vivamarks;
            cout << "Enter Exam Name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter Date: ";
            cin >> date;
            cout << "Enter viva marks: ";
            cin >> vivamarks;
            sched.addExam(new Practical(name, date, vivamarks));
            break;
        }
        case 4:
        {
            string sid, sname;
            cout << "Enter Student ID: ";
            cin >> sid;
            cout << "Enter Student Name: ";
            cin.ignore();
            getline(cin, sname);
            sched.addStudents(new Student(sid, sname));
            break;
        }
        case 5:
        {
            string ename, rid;
            cout << "Enter Exam name: ";
            cin >> ename;
            cout << "Enter Classroom ID: ";
            cin >> rid;
            sched.assignClassroomToExam(ename, rid);
            break;
        }
        case 6:
            sched.finalSchedule();
            break;
        case 7:
            sched.viewStudentTimetables();
            break;
        case 8:
        {
            string sid, ename;
            cout << "Enter Student ID: ";
            cin >> sid;
            cout << "Enter Exam Name: ";
            cin >> ename;
            sched.assignExamtoStud(sid, ename);
            break;
        }
        case 9:
            sched.showExamseating();
            break;
        case 10:
            exit(0);
        default:
            cout << "Invalid choice!\n";
        }
    }
    return 0;
}