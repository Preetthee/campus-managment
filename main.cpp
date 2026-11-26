#include <iostream>
#include <string>
#include "student_ranking.h"
#include "student_search.h"
#include "campus_navigation.h"
#include "course_scheduling.h"
using namespace std;

void showMenu() {
    cout << "\n=== Intelligent Campus Management System ===\n";
    cout << "1. Student Search\n";
    cout << "2. Course Scheduling\n";
    cout << "3. Campus Navigation\n";
    cout << "4. Student Ranking\n";
    cout << "0. Exit\n";
    cout << "Choose an option: ";
}

int main() {
    string choice;
    while (true) {
        showMenu();
        if (!getline(cin, choice)) break;
        if (choice == "0") {
            cout << "Goodbye.\n";
            break;
        }
        cout << "\n";
        if (choice == "1") runStudentSearch();
        else if (choice == "2")runCourseScheduling();
        else if (choice == "3") runCampusNavigation();
        else if (choice == "4") runStudentRanking();
        
        
        else cout << "Invalid option.\n";

    
    }
    return 0;
}