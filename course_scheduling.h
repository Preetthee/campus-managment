#pragma once
#include <string>
#include <vector>

struct Course {
    std::string day;
    std::string code;
    std::string name;
    std::string teacher;
    std::string room;
    std::string start;
    std::string end;
};

std::vector<Course> loadCourses(const std::string& path);
std::vector<Course> scheduleCourses(std::vector<Course> courses);
void runCourseScheduling();