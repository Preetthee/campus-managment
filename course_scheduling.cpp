#include "course_scheduling.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

static bool isNumber(const string& text) {
    if (text.empty() || text.size() > 6) return false;
    for (char c : text) {
        if (c < '0' || c > '9') return false;
    }
    return true;
}

static int toMinutes(const string& t) {
    return stoi(t.substr(0, 2)) * 60 + stoi(t.substr(3, 2));
}

static vector<string> splitCsv(const string& line) {
    vector<string> parts;
    string current;
    bool inQuotes = false;
    for (char c : line) {
        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) {
            parts.push_back(current);
            current.clear();
        } else {
            current += c;
        }
    }
    parts.push_back(current);
    return parts;
}

vector<Course> loadCourses(const string& path) {
    vector<Course> courses;
    ifstream file(path);
    string line;
    getline(file, line);
    while (getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        vector<string> p = splitCsv(line);
        if (p.size() < 7) continue;
        Course c;
        c.day = p[0];
        c.code = p[1];
        c.name = p[2];
        c.teacher = p[3];
        c.room = p[4];
        c.start = p[5];
        c.end = p[6];
        courses.push_back(c);
    }
    return courses;
}

vector<Course> scheduleCourses(vector<Course> courses) {
    stable_sort(courses.begin(), courses.end(), [](const Course& a, const Course& b) {
        return toMinutes(a.end) < toMinutes(b.end);
    });
    vector<Course> chosen;
    int lastEnd = 0;
    for (const Course& c : courses) {
        if (toMinutes(c.start) >= lastEnd) {
            chosen.push_back(c);
            lastEnd = toMinutes(c.end);
        }
    }
    return chosen;
}

void runCourseScheduling() {
    vector<Course> courses = loadCourses("data/courses.csv");
    if (courses.empty()) {
        cout << "No course data found in data/courses.csv\n";
        return;
    }
    const vector<string> days = {"Saturday", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday"};
    for (size_t i = 0; i < days.size(); i++) {
        cout << i + 1 << ". " << days[i] << "\n";
    }
    cout << "Choose a day (number): ";
    string text;
    getline(cin, text);
    if (!isNumber(text) || stoi(text) < 1 || stoi(text) > (int)days.size()) {
        cout << "Invalid choice.\n";
        return;
    }
    string day = days[stoi(text) - 1];
    vector<Course> today;
    for (const Course& c : courses) {
        if (c.day == day) today.push_back(c);
    }
    if (today.empty()) {
        cout << "No classes on " << day << "\n";
        return;
    }
    vector<Course> chosen = scheduleCourses(today);
    sort(chosen.begin(), chosen.end(), [](const Course& a, const Course& b) {
        return toMinutes(a.start) < toMinutes(b.start);
    });
    cout << "\nSchedule for " << day << "\n";
    cout << string(70, '-') << "\n";
    for (const Course& c : chosen) {
        cout << c.start << " - " << c.end << "  " << c.code << "  " << c.name
             << "  (Room " << c.room << ")\n";
    }
    cout << string(70, '-') << "\n";
    cout << chosen.size() << " of " << today.size() << " classes scheduled\n";
}