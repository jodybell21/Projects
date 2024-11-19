#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <map>
using namespace std;

// Forward declarations
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

vector<Course*> getCoursesForYear(int year) {
    vector<Course*> courses;

    switch (year) {
    case 1:
        courses.push_back(new Course(101, "Information Systems", "Classroom", 40));
        courses.push_back(new Course(102, "Programming", "Lab", 30));
        courses.push_back(new Course(103, "Web Technology", "Lab", 30));
        courses.push_back(new Course(104, "Networks", "Lab", 30));
        courses.push_back(new Course(105, "Mathematics", "Classroom", 40));
        break;

    case 2:
        courses.push_back(new Course(201, "Information Systems", "Classroom", 40));
        courses.push_back(new Course(202, "Programming", "Lab", 30));
        courses.push_back(new Course(203, "Database Systems", "Lab", 30));
        courses.push_back(new Course(204, "Cloud Computing", "Lab", 30));
        courses.push_back(new Course(205, "Internet Computing", "Lab", 30));
        break;

    case 3:
        courses.push_back(new Course(301, "Software Engineering", "Classroom", 40));
        courses.push_back(new Course(302, "Programming", "Lab", 30));
        courses.push_back(new Course(303, "Cyber Security", "Lab", 30));
        courses.push_back(new Course(304, "Artificial Intelligence", "Lab", 30));
        courses.push_back(new Course(305, "Machine Learning", "Lab", 30));
        break;

    default:
        cout << "Invalid year\n";
    }

    return courses;
}

struct TimeSlot {
    int day;    // 1-5 for Monday-Friday
    int period; // 1-8 for different periods in a day

    bool operator==(const TimeSlot& other) const {
        return day == other.day && period == other.period;
    }

    string toString() const {
        string days[] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday" };
        return days[day - 1] + ", Period " + to_string(period);
    }
};

struct ScheduleEntry {
    Course* course;
    Room* room;
    TimeSlot timeSlot;
};

class ScheduleOptimizer {
private:
    vector<Room*>& rooms;
    vector<TimeSlot> availableTimeSlots;
    map<Room*, vector<TimeSlot>> roomSchedule;
    map<Student*, vector<ScheduleEntry>> studentSchedules;
    map<Course*, vector<Student*>> courseEnrollments;

    // Initialize available time slots (Monday-Friday, 8 periods each)
    void initializeTimeSlots() {
        for (int day = 1; day <= 5; day++) {
            for (int period = 1; period <= 8; period++) {
                availableTimeSlots.push_back({ day, period });
            }
        }
    }

    bool hasScheduleConflict(Student* student, TimeSlot newSlot) {
        if (studentSchedules.find(student) == studentSchedules.end()) {
            return false;
        }

        for (const ScheduleEntry& entry : studentSchedules[student]) {
            if (entry.timeSlot == newSlot) {
                return true;
            }
        }
        return false;
    }

    bool isRoomAvailable(Room* room, TimeSlot slot) {
        if (roomSchedule.find(room) == roomSchedule.end()) {
            return true;
        }

        for (const TimeSlot& occupied : roomSchedule[room]) {
            if (occupied == slot) {
                return false;
            }
        }
        return true;
    }

    // Find best room based on capacity and equipment needs
    Room* findOptimalRoom(Course* course, TimeSlot slot) {
        Room* bestRoom = nullptr;
        int minWastedSpace = INT_MAX;

        for (Room* room : rooms) {
            if (room->getType() == course->getRequiredRoom() &&
                room->getCapacity() >= course->getMaxCapacity() &&
                isRoomAvailable(room, slot)) {

                int wastedSpace = room->getCapacity() - course->getMaxCapacity();
                if (wastedSpace < minWastedSpace) {
                    minWastedSpace = wastedSpace;
                    bestRoom = room;
                }
            }
        }
        return bestRoom;
    }

    // Find optimal time slot considering student availability
    TimeSlot findOptimalTimeSlot(Course* course, const vector<Student*>& students) {
        vector<TimeSlot> possibleSlots = availableTimeSlots;

        // Sort time slots by number of student conflicts (ascending)
        sort(possibleSlots.begin(), possibleSlots.end(),
            [this, &students](const TimeSlot& a, const TimeSlot& b) {
                int conflictsA = 0, conflictsB = 0;
                for (Student* student : students) {
                    if (hasScheduleConflict(student, a)) conflictsA++;
                    if (hasScheduleConflict(student, b)) conflictsB++;
                }
                return conflictsA < conflictsB;
            });

        for (const TimeSlot& slot : possibleSlots) {
            if (findOptimalRoom(course, slot) != nullptr) {
                return slot;
            }
        }

        return possibleSlots[0]; // Fallback to first available slot
    }

public:
    ScheduleOptimizer(vector<Room*>& rooms) : rooms(rooms) {
        initializeTimeSlots();
    }

    bool scheduleRegistration(RegistrationRequest& request) {
        Course* course = request.course;
        Student* student = request.student;

        // Add student to course enrollments
        if (courseEnrollments.find(course) == courseEnrollments.end()) {
            courseEnrollments[course] = vector<Student*>();
        }

        // Check if course is already at capacity
        if (courseEnrollments[course].size() >= course->getMaxCapacity()) {
            return false;
        }

        // If this is the first student, find optimal room and time slot
        if (courseEnrollments[course].empty()) {
            vector<Student*> potentialStudents = { student };
            TimeSlot optimalSlot = findOptimalTimeSlot(course, potentialStudents);
            Room* optimalRoom = findOptimalRoom(course, optimalSlot);

            if (optimalRoom != nullptr) {
                // Create schedule entry
                ScheduleEntry entry = { course, optimalRoom, optimalSlot };

                // Update schedules
                if (studentSchedules.find(student) == studentSchedules.end()) {
                    studentSchedules[student] = vector<ScheduleEntry>();
                }
                studentSchedules[student].push_back(entry);
                courseEnrollments[course].push_back(student);

                if (roomSchedule.find(optimalRoom) == roomSchedule.end()) {
                    roomSchedule[optimalRoom] = vector<TimeSlot>();
                }
                roomSchedule[optimalRoom].push_back(optimalSlot);

                return true;
            }
        }
        else {
            // Course already scheduled, check if student can join
            ScheduleEntry existingEntry = studentSchedules[courseEnrollments[course][0]][0];
            if (!hasScheduleConflict(student, existingEntry.timeSlot)) {
                studentSchedules[student].push_back(existingEntry);
                courseEnrollments[course].push_back(student);
                return true;
            }
        }

        return false;
    }

    void printStudentSchedule(Student* student) {
        cout << "\nSchedule for Student ID " << student->getStudentId() << ":\n";
        if (studentSchedules.find(student) != studentSchedules.end()) {
            for (const ScheduleEntry& entry : studentSchedules[student]) {
                cout << entry.course->getName() << "\n"
                    << "  Room: " << entry.room->getRoomNumber() << "\n"
                    << "  Time: " << entry.timeSlot.toString() << "\n"
                    << "  Current Enrollment: " << courseEnrollments[entry.course].size()
                    << "/" << entry.course->getMaxCapacity() << "\n\n";
            }
        }
    }
};

int main() {
    // Create rooms
    vector<Room*> rooms;
    rooms.push_back(new Room(201, "Classroom", 40, "Whiteboard"));
    rooms.push_back(new Room(202, "Lab", 30, "Computers"));
    rooms.push_back(new Room(203, "Classroom", 35, "Whiteboard"));
    rooms.push_back(new Room(204, "Lab", 25, "Computers"));

    // Initialize scheduler
    ScheduleOptimizer scheduler(rooms);
    priority_queue<RegistrationRequest> regQueue;

    // Get student info
    int year;
    cout << "What year will you be going into (1-3): ";
    cin >> year;

    if (year >= 1 && year <= 3) {
        vector<Course*> availableCourses = getCoursesForYear(year);

        cout << "\nAvailable courses for Year " << year << ":\n";
        for (size_t i = 0; i < availableCourses.size(); i++) {
            cout << i + 1 << ". " << availableCourses[i]->getName() << endl;
        }

        Student* student = new Student(1, "Computer Science", year);

        // Course selection
        cout << "\nSelect courses (enter course numbers, -1 to finish):\n";
        while (true) {
            int choice;
            cout << "Enter course number: ";
            cin >> choice;

            if (choice == -1) break;

            if (choice >= 1 && choice <= static_cast<int>(availableCourses.size())) {
                Course* selectedCourse = availableCourses[choice - 1];

                RegistrationRequest req;
                req.student = student;
                req.course = selectedCourse;
                req.isCoreCourse = (year == 1);
                req.timestamp = time(nullptr);
                regQueue.push(req);
            }
            else {
                cout << "Invalid course number\n";
            }
        }

        // Process registration requests
        cout << "\nProcessing registration requests...\n";
        while (!regQueue.empty()) {
            RegistrationRequest current = regQueue.top();
            regQueue.pop();

            if (scheduler.scheduleRegistration(current)) {
                cout << "Successfully enrolled in " << current.course->getName() << "\n";
            }
            else {
                cout << "Could not enroll in " << current.course->getName()
                    << " (Schedule conflict or course full)\n";
            }
        }

        // Print final schedule
        scheduler.printStudentSchedule(student);

        // Cleanup
        for (Course* course : availableCourses) {
            delete course;
        }
        delete student;
    }
    else {
        cout << "Invalid year. Please enter a year between 1 and 3." << endl;
    }

    // Cleanup rooms
    for (Room* room : rooms) {
        delete room;
    }

    return 0;
}