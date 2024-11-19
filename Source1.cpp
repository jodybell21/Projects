#include <iostream>
#include <string>
#include <vector>
#include <queue>
using namespace std;

class Student;
class Room;
class Course;

class Course {
private:
    int courseCode;
    string name;
    string requiredRoom;
    int maxCapacity;
    vector<Student*> enrolledStudents;
    vector<int> scheduleTimeSlots;
    Room* assignedRoom;
public:
    Course(int courseCode, string name, string requiredRoom, int maxCapacity) {
        this->courseCode = courseCode;
        this->name = name;
        this->requiredRoom = requiredRoom;
        this->maxCapacity = maxCapacity;
        this->assignedRoom = nullptr;
    }

    void setCourseCode(int courseCode) {
        this->courseCode = courseCode;
    }

    int getCourseCode() const {
        return courseCode;
    }

    void setName(string name) {
        this->name = name;
    }

    string getName() const {
        return name;
    }

    void setRequiredRoom(string requiredRoom) {
        this->requiredRoom = requiredRoom;
    }

    string getRequiredRoom() const {
        return requiredRoom;
    }

    void setMaxCapacity(int maxCapacity) {
        this->maxCapacity = maxCapacity;
    }

    int getMaxCapacity() const {
        return maxCapacity;
    }

    int getEnrolledStudents() const {
        return enrolledStudents.size();
    }

    vector<int> getScheduleTimeslots() const {
        return scheduleTimeSlots;
    }

    bool addStudent(Student* student) {
        if (enrolledStudents.size() < maxCapacity) {
            enrolledStudents.push_back(student);
            return true;
        }
        return false;
    }

    void addTimeslot(int timeslot) {
        scheduleTimeSlots.push_back(timeslot);
    }

    bool isFull() const {
        return enrolledStudents.size() >= maxCapacity;
    }

    bool hasTimeConflict(int newTimeslot) const {
        for (int slot : scheduleTimeSlots) {
            if (slot == newTimeslot) return true;
        }
        return false;
    }
};

class Student {
private:
    int studentId;
    string major;
    vector<Course*> enrolledCourses;
    int academicYear;
public:
    Student(int studentId, string major, int academicYear) {
        this->studentId = studentId;
        this->major = major;
        this->academicYear = academicYear;
    }

    void setStudentId(int studentId) {
        this->studentId = studentId;
    }

    int getStudentId() const {
        return studentId;
    }

    void setMajor(string major) {
        this->major = major;
    }

    string getMajor() const {
        return major;
    }

    void setAcademicYear(int academicYear) {
        this->academicYear = academicYear;
    }

    int getAcademicYear() const {
        return academicYear;
    }

    bool enrollInCourse(Course* course) {
        if (course->addStudent(this)) {
            enrolledCourses.push_back(course);
            return true;
        }
        return false;
    }
};

class Room {
private:
    int roomNumber;
    string type;
    int capacity;
    string specialEquipment;
    vector<int> availableTimeSlots;
public:
    Room(int roomNumber, string type, int capacity, string specialEquipment) {
        this->roomNumber = roomNumber;
        this->type = type;
        this->capacity = capacity;
        this->specialEquipment = specialEquipment;
    }

    void setRoomNumber(int roomNumber) {
        this->roomNumber = roomNumber;
    }

    int getRoomNumber() const {
        return roomNumber;
    }

    void setType(string type) {
        this->type = type;
    }

    string getType() const {
        return type;
    }

    void setCapacity(int capacity) {
        this->capacity = capacity;
    }

    int getCapacity() const {
        return capacity;
    }

    void setSpecialEquipment(string specialEquipment) {
        this->specialEquipment = specialEquipment;
    }

    string getSpecialEquipment() const {
        return specialEquipment;
    }

    bool isAvailable(int timeSlot) const {
        for (int occupied : availableTimeSlots) {
            if (occupied == timeSlot) return false;
        }
        return true;
    }
};

struct RegistrationRequest {
    Student* student;
    Course* course;
    bool isCoreCourse;
    time_t timestamp;

    bool operator<(const RegistrationRequest& other) const {
        // First check year
        if (student->getAcademicYear() != other.student->getAcademicYear()) {
            return student->getAcademicYear() < other.student->getAcademicYear();
        }
        // Then check if it's a core course
        if (isCoreCourse != other.isCoreCourse) {
            return !isCoreCourse;
        }
        // Finally check timestamp
        return timestamp > other.timestamp;
    }
};

bool hasScheduleConflict(vector<int>& schedule, int newTimeslot) {
    for (int slot : schedule) {
        if (slot == newTimeslot) {
            return true;
        }
    }
    return false;
}

bool isRoomSuitable(Room* room, Course* course) {
    return (room->getType() == course->getRequiredRoom() &&
        room->getCapacity() >= course->getMaxCapacity());
}

void showStudentYears(priority_queue<int> gq) {
    priority_queue<int> g = gq;
    while (!g.empty()) {
        cout << ' ' << g.top();
        g.pop();
    }
    cout << '\n';
}

int main() {
    priority_queue<RegistrationRequest> regQueue;
    vector<Room*> rooms;
    vector<int> studentSchedule;

    // Create test courses
    Course* prog101 = new Course(101, "Programming 101", "Lab", 30);
    Course* math101 = new Course(102, "Math 101", "Classroom", 40);

    // Create test rooms
    Room* room1 = new Room(201, "Lab", 30, "Computers");
    Room* room2 = new Room(202, "Classroom", 40, "Whiteboard");
    rooms.push_back(room1);
    rooms.push_back(room2);

    // Get student info
    int year;
    cout << "What year will you be going into (1-4): ";
    cin >> year;

    priority_queue<int> studentYears;
    studentYears.push(year);
    showStudentYears(studentYears);

    if (year >= 1 && year <= 4) {
        Student* student1 = new Student(1, "Computer Science", year);

        // Create registration requests
        RegistrationRequest req1;
        req1.student = student1;
        req1.course = prog101;
        req1.isCoreCourse = true;
        req1.timestamp = time(nullptr);
        regQueue.push(req1);

        RegistrationRequest req2;
        req2.student = student1;
        req2.course = math101;
        req2.isCoreCourse = false;
        req2.timestamp = time(nullptr);
        regQueue.push(req2);

        // Process registration requests
        cout << "\nProcessing registration requests...\n";
        while (!regQueue.empty()) {
            RegistrationRequest current = regQueue.top();
            regQueue.pop();

            // Find suitable room
            Room* assignedRoom = nullptr;
            for (Room* room : rooms) {
                if (isRoomSuitable(room, current.course)) {
                    assignedRoom = room;
                    break;
                }
            }

            // Try to assign a timeslot
            bool scheduled = false;
            if (assignedRoom != nullptr) {
                for (int slot = 1; slot <= 4; slot++) {
                    if (!hasScheduleConflict(studentSchedule, slot)) {
                        studentSchedule.push_back(slot);
                        current.course->addTimeslot(slot);
                        scheduled = true;
                        break;
                    }
                }
            }

            // Try to enroll
            if (scheduled && current.student->enrollInCourse(current.course)) {
                cout << "Successfully enrolled in " << current.course->getName()
                    << " (Timeslot: " << current.course->getScheduleTimeslots().back() << ")\n";
            }
            else {
                cout << "Could not enroll in " << current.course->getName() << "\n";
            }
        }
    }
    else {
        cout << "Invalid year. Please enter a year between 1 and 4." << endl;
    }

    // Cleanup
    delete prog101;
    delete math101;
    delete room1;
    delete room2;

    return 0;
}