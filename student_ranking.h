#pragma once
#include <string>
#include <vector>

struct RankedStudent {
    long long id;
    std::string name;
    double cgpa;
};

std::vector<RankedStudent> loadRankingData(const std::string& studentsPath,
                                           const std::string& resultsPath);
std::vector<RankedStudent> mergeSort(std::vector<RankedStudent> students);
void runStudentRanking();