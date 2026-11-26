#include "campus_navigation.h"
#include <climits>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>

using namespace std;

static bool isNumber(const string& text) {
    if (text.empty() || text.size() > 6) return false;
    for (char c : text) {
        if (c < '0' || c > '9') return false;
    }
    return true;
}

Graph loadGraph(const string& path) {
    Graph graph;
    ifstream file(path);
    string line;
    getline(file, line);
    while (getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        size_t first = line.find(',');
        size_t second = line.rfind(',');
        if (first == string::npos || first == second) continue;
        string a = line.substr(0, first);
        string b = line.substr(first + 1, second - first - 1);
        int d = stoi(line.substr(second + 1));
        graph[a].push_back({b, d});
        graph[b].push_back({a, d});
    }
    return graph;
}

int shortestPath(const Graph& graph, const string& start, const string& end,
                 vector<string>& path) {
    map<string, int> dist;
    map<string, string> prev;
    for (const auto& entry : graph) dist[entry.first] = INT_MAX;
    dist[start] = 0;

    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> heap;
    heap.push({0, start});

    while (!heap.empty()) {
        int d = heap.top().first;
        string node = heap.top().second;
        heap.pop();
        if (d > dist[node]) continue;
        if (node == end) break;
        for (const Edge& e : graph.at(node)) {
            int newDist = d + e.weight;
            if (newDist < dist[e.to]) {
                dist[e.to] = newDist;
                prev[e.to] = node;
                heap.push({newDist, e.to});
            }
        }
    }

    if (dist[end] == INT_MAX) return -1;

    path.clear();
    string current = end;
    path.push_back(current);
    while (current != start) {
        current = prev[current];
        path.push_back(current);
    }
    vector<string> reversed(path.rbegin(), path.rend());
    path = reversed;
    return dist[end];
}

static string pickPlace(const vector<string>& places, const string& label) {
    while (true) {
        cout << label << " (number): ";
        string text;
        if (!getline(cin, text)) return "";
        if (isNumber(text)) {
            int n = stoi(text);
            if (n >= 1 && n <= (int)places.size()) return places[n - 1];
        }
        cout << "Invalid choice, try again.\n";
    }
}

void runCampusNavigation() {
    Graph graph = loadGraph("data/campus_map.csv");
    if (graph.empty()) {
        cout << "No map data found in data/campus_map.csv\n";
        return;
    }
    vector<string> places;
    for (const auto& entry : graph) places.push_back(entry.first);

    cout << "Campus buildings\n";
    for (size_t i = 0; i < places.size(); i++) {
        cout << i + 1 << ". " << places[i] << "\n";
    }
    string start = pickPlace(places, "From");
    if (start.empty()) return;
    string end = pickPlace(places, "To");
    if (end.empty()) return;

    vector<string> path;
    int distance = shortestPath(graph, start, end, path);
    if (distance == -1) {
        cout << "No route found.\n";
        return;
    }
    cout << "Route   : ";
    for (size_t i = 0; i < path.size(); i++) {
        if (i > 0) cout << " -> ";
        cout << path[i];
    }
    cout << "\nDistance: " << distance << " m\n";
}