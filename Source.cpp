#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <unordered_set>

using namespace std;

class ClubHub {
private:
    unordered_map<string, vector<string>> graph;
    unordered_map<string, vector<string>> studentClubs;
    vector<string> studentNames;

    string getLastName(const string& fullName) {
        size_t lastSpacePos = fullName.find_last_of(' ');
        return (lastSpacePos != string::npos) ? fullName.substr(lastSpacePos + 1) : fullName;
    }

    // Insertion sort for student names
    void insertionSortStudentNames() {
        for (int i = 1; i < studentNames.size(); i++) {
            string key = studentNames[i];
            string keyLastName = getLastName(key);
            int j = i - 1;

            while (j >= 0 && getLastName(studentNames[j]) > keyLastName) {
                studentNames[j + 1] = studentNames[j];
                j--;
            }
            studentNames[j + 1] = key;
        }
    }

public:

    // Save system state to a file
    bool saveToFile(const string& fileName) {
        ofstream outFile(fileName);
        if (!outFile) {
            cout << "Error: Unable to open file " << fileName << " for writing." << endl;
            return false;
        }

        // Save students
        outFile << "STUDENTS:" << endl;
        for (const auto& student : studentNames) {
            outFile << student << endl;
        }
        outFile << "END_STUDENTS" << endl;

        // Save clubs
        outFile << "CLUBS:" << endl;
        // Use a set to get unique clubs
        unordered_set<string> uniqueClubs;
        for (const auto& entry : studentClubs) {
            for (const auto& club : entry.second) {
                uniqueClubs.insert(club);
            }
        }
        for (const auto& club : uniqueClubs) {
            outFile << club << endl;
        }
        outFile << "END_CLUBS" << endl;

        // Save club memberships
        outFile << "MEMBERSHIPS:" << endl;
        for (const auto& entry : studentClubs) {
            const string& student = entry.first;
            for (const auto& club : entry.second) {
                outFile << student << ":" << club << endl;
            }
        }
        outFile << "END_MEMBERSHIPS" << endl;

        outFile.close();
        cout << "System state saved to " << fileName << endl;
        return true;
    }

    // Load system state from a file
    bool loadFromFile(const string& fileName) {
        // Clear existing data
        graph.clear();
        studentClubs.clear();
        studentNames.clear();

        ifstream inFile(fileName);
        if (!inFile) {
            cerr << "Error: Unable to open file " << fileName << " for reading." << endl;
            return false;
        }

        string line;
        string section;

        while (getline(inFile, line)) {
            // Trim whitespace
            line.erase(0, line.find_first_not_of(" \t"));
            line.erase(line.find_last_not_of(" \t") + 1);

            // Check for section headers
            if (line == "STUDENTS:") {
                section = "STUDENTS";
                continue;
            }
            else if (line == "CLUBS:") {
                section = "CLUBS";
                continue;
            }
            else if (line == "MEMBERSHIPS:") {
                section = "MEMBERSHIPS";
                continue;
            }
            else if (line == "END_STUDENTS" || line == "END_CLUBS" || line == "END_MEMBERSHIPS") {
                section = "";
                continue;
            }
            if (section == "STUDENTS") {
                addStudent(line);
            }
            else if (section == "CLUBS") {
                createClub(line);
            }
            else if (section == "MEMBERSHIPS") {
                // Split student:club
                size_t colonPos = line.find(':');
                if (colonPos != string::npos) {
                    string studentName = line.substr(0, colonPos);
                    string clubName = line.substr(colonPos + 1);

                    if (studentClubs.find(studentName) != studentClubs.end()) {
                        addStudentToClub(studentName, clubName);
                    }
                }
            }
        }

        inFile.close();
        cout << "System state loaded from " << fileName << endl;
        return true;
    }



    void addStudent(string studentName) {
        studentClubs[studentName] = {};
    }

    void createClub(string clubName) {
        for (auto it = studentClubs.begin(); it != studentClubs.end(); ++it) {
            it->second.push_back(clubName);
            graph[it->first].push_back(clubName);
        }
    }

    void addStudentToClub(string studentName, string clubName) {
        studentClubs[studentName].push_back(clubName);
        graph[studentName].push_back(clubName);
        for (const auto& member : studentClubs[clubName]) {
            if (member != studentName) {
                graph[studentName].push_back(member);
                graph[member].push_back(studentName);
            }
        }
        studentClubs[clubName].push_back(studentName);
    }


    void removeStudentFromClub(string studentName, string clubName) {
        // Remove club from student's list
        auto& studentClubList = studentClubs[studentName];
        studentClubList.erase(remove(studentClubList.begin(), studentClubList.end(), clubName),
            studentClubList.end());

        // Remove student from club's list
        auto& clubMemberList = studentClubs[clubName];
        clubMemberList.erase(remove(clubMemberList.begin(), clubMemberList.end(), studentName),
            clubMemberList.end());

        // Remove connections from the graph
        auto& studentConnections = graph[studentName];
        studentConnections.erase(remove(studentConnections.begin(), studentConnections.end(), clubName),
            studentConnections.end());

        // Remove connections with other club members
        for (const auto& member : studentClubs[clubName]) {
            if (member != studentName) {
                
                auto& connections = graph[studentName];
                connections.erase(remove(connections.begin(), connections.end(), member),
                    connections.end());

                auto& memberConnections = graph[member];
                memberConnections.erase(remove(memberConnections.begin(), memberConnections.end(), studentName),
                    memberConnections.end());
            }
        }
    }

    // Search Algorithm 1
    vector<string> findStudentClubs(string studentName) {
        auto it = studentClubs.find(studentName);
        if (it != studentClubs.end()) {
            vector<string> uniqueClubs;
            for (const auto& club : it->second) {
                if (find(uniqueClubs.begin(), uniqueClubs.end(), club) == uniqueClubs.end()) {
                    uniqueClubs.push_back(club);
                }
            }
            return uniqueClubs;
        }
       
        return {};
    }

    void printStudentsSortedByLastName() {
        cout << "Students sorted by last name:" << endl;
        for (const auto& student : studentNames) {
            cout << student << endl;
        }
    }

    void printConnections() {
        for (const auto& pair : graph) {
            for (const auto& club : pair.second) {
                cout << "There is a connection between " << pair.first << " and " << club << endl;
            }
        }
    }
};

int main() {
    ClubHub club;
    
    club.addStudent("John Smith");
    club.addStudent("Alice Johnson");
    club.addStudent("Bob Williams");

    // Create some clubs
    club.createClub("Chess Club");
    club.createClub("Debate Club");
    club.createClub("Gaming Club");
    club.createClub("Photography Club");

    // Add students to clubs
    club.addStudentToClub("Jody Bell", "Gaming Club");
    club.addStudentToClub("Litha Madlingozi", "Debate Club");
    club.addStudentToClub("Tiana Wessels", "Chess Club");
    club.addStudentToClub("Tiana Wessels", "Debate Club");

    club.printConnections();

    // Save the current state
    club.saveToFile("club_system_state.txt");

    // Create a new ClubHub instance and load the saved state
    ClubHub newClub;
    newClub.loadFromFile("club_system_state.txt");
    cout << "Here are the students" << endl;
    newClub.printStudentsSortedByLastName();
    

    return 0;
}
