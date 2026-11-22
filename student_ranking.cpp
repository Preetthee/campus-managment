#include "student_ranking.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>

using namespace std;

vector<RankedStudent> loadRankingData(const string& studentsPath, const string& resultsPath) {
    map<long long, string> names;
    ifstream students(studentsPath);
    string line;
    getline(students, line);
    while (getline(students, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        size_t comma = line.find(',');
        if (comma == string::npos) continue;
        names[stoll(line.substr(0, comma))] = line.substr(comma + 1);
    }

    vector<RankedStudent> result;
    ifstream results(resultsPath);
    getline(results, line);
    while (getline(results, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        size_t comma = line.find(',');
        if (comma == string::npos) continue;
        long long id = stoll(line.substr(0, comma));
        auto found = names.find(id);
        if (found == names.end()) continue;
        result.push_back({id, found->second, stod(line.substr(comma + 1))});
    }
    return result;
}

static bool better(const RankedStudent& a, const RankedStudent& b) {
    if (a.cgpa != b.cgpa) return a.cgpa > b.cgpa;
    return a.id < b.id;
}

static vector<RankedStudent> merge(const vector<RankedStudent>& left,
                                   const vector<RankedStudent>& right) {
    vector<RankedStudent> result;
    size_t i = 0;
    size_t j = 0;
    while (i < left.size() && j < right.size()) {
        if (better(left[i], right[j])) result.push_back(left[i++]);
        else result.push_back(right[j++]);
    }
    while (i < left.size()) result.push_back(left[i++]);
    while (j < right.size()) result.push_back(right[j++]);
    return result;
}

vector<RankedStudent> mergeSort(vector<RankedStudent> students) {
    if (students.size() <= 1) return students;
    size_t mid = students.size() / 2;
    vector<RankedStudent> left(students.begin(), students.begin() + mid);
    vector<RankedStudent> right(students.begin() + mid, students.end());
    return merge(mergeSort(left), mergeSort(right));
}

void runStudentRanking() {
    vector<RankedStudent> students = loadRankingData("data/students.csv", "data/results.csv");
    if (students.empty()) {
        cout << "No result data found in data/results.csv\n";
        return;
    }
    vector<RankedStudent> ranked = mergeSort(students);
    cout << left << setw(6) << "Rank" << setw(12) << "ID" << setw(32) << "Name" << "CGPA\n";
    cout << string(58, '-') << "\n";
    for (size_t i = 0; i < ranked.size(); i++) {
        cout << left << setw(6) << i + 1 << setw(12) << ranked[i].id << setw(32)
             << ranked[i].name << fixed << setprecision(2) << ranked[i].cgpa << "\n";
    }
}