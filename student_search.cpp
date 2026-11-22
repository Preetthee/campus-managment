#include "student_search.h"
#include <algorithm>
#include <fstream>
#include <iostream>

using namespace std;

static bool isNumber(const string& text) {
    if (text.empty() || text.size() > 18) return false;
    for (char c : text) {
        if (c < '0' || c > '9') return false;
    }
    return true;
}

vector<Student> loadStudents(const string& path) {
    vector<Student> students;
    ifstream file(path);
    string line;
    getline(file, line);
    while (getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        size_t comma = line.find(',');
        if (comma == string::npos) continue;
        Student s;
        s.id = stoll(line.substr(0, comma));
        s.name = line.substr(comma + 1);
        students.push_back(s);
    }
    sort(students.begin(), students.end(),
         [](const Student& a, const Student& b) { return a.id < b.id; });
    return students;
}

int binarySearch(const vector<Student>& students, long long targetId) {
    int low = 0;
    int high = (int)students.size() - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (students[mid].id == targetId) return mid;
        if (students[mid].id < targetId) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

void runStudentSearch() {
    vector<Student> students = loadStudents("data/students.csv");
    if (students.empty()) {
        cout << "No student data found in data/students.csv\n";
        return;
    }
    cout << "Enter student ID: ";
    string text;
    getline(cin, text);
    if (!isNumber(text)) {
        cout << "Invalid ID.\n";
        return;
    }
    int index = binarySearch(students, stoll(text));
    if (index == -1) {
        cout << "No student found with ID " << text << "\n";
        return;
    }
    cout << "Student found\n";
    cout << "ID  : " << students[index].id << "\n";
    cout << "Name: " << students[index].name << "\n";
}