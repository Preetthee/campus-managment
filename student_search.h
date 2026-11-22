#pragma once
#include <string>
#include <vector>

struct Student {
    long long id;
    std::string name;
};

std::vector<Student> loadStudents(const std::string& path);
int binarySearch(const std::vector<Student>& students, long long targetId);
void runStudentSearch();