#pragma once
#include <map>
#include <string>
#include <vector>

struct Edge {
    std::string to;
    int weight;
};

using Graph = std::map<std::string, std::vector<Edge>>;

Graph loadGraph(const std::string& path);
int shortestPath(const Graph& graph, const std::string& start, const std::string& end,
                 std::vector<std::string>& path);
void runCampusNavigation();