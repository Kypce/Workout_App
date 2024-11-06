/* WORKOUT INTERVAL APP
    Author: Matthew Class
    About: C++ application that lets a user create interval workouts and save them for future use
*/

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <chrono>
#include <ctime>
#include <thread>

std::string effortInterpret(char eff);

struct data {
    std::string name, description;
    std::vector<char> effort;
    std::vector<std::string> intervals;
    
    void printData() {
        std::cout << "Name: " << name << "\nDescription: " << description << "\n";
        for(int i = 0; i < intervals.size(); i++) {
            std::cout << "[" << effortInterpret(effort[i]) << ": " << intervals[i] << "] ";
            if(i % 10 == 0 && i != 0)
                std::cout << "\n";
        }
        std::cout << "\n\n";
    }

    // Formats struct data for ouput to file
    std::string format() {
        std::string out;
        std::string inter;
        
        std::string eff(effort.begin(), effort.end());
        for(int i = 0; i < intervals.size(); i++) {
            inter += intervals[i] + '#';
        }
        inter.pop_back();

        // Commas used as delimeter to be able to divide data when pulled again
        out = name + ',' + description + ',' + eff + ',' + inter + '\n';

        return out;
    }
};

// Takes single character from effort vector and interprets it to meaning
std::string effortInterpret(char eff) {
    std::string out;
    switch(eff) {
        case('r'):
        case('R'):
             out = "Walking recovery";
            break;
         case('b'):
         case('B'):
               out = "Base pace";
              break;
           case('p'):
           case('P'):
              out = "Push pace";
               break;
           case('a'):
           case('A'):
              out = "All-out";
              break;
    }

    return out;
}

// Converts seconds to minutes
double secondsToMinutes(int sec) {
    if(sec <= 60)
        return sec;
    else
        return sec/60.0;
}

// Checks if given string is a number or not
bool checkNumber(std::string str) {
    for(int i = 0; i < str.length(); i++) {
        if(std::isdigit(str[i]) == false)
            return false;
    }
    return true;
}

// Takes in an int length and will count down from that time
void stopwatch(int length, char nextEff = ' ', int nextInt = -1) {
    std::chrono::seconds totalLength(length);
    std::chrono::seconds second(1);

    // Can use this to be able to do an action every second
    for(length; length >= 0; length--) {
        std::this_thread::sleep_for(second);
        if(length % 60 == 0 && length != 60) {
            std::cout << secondsToMinutes(length) << " minutes left!\n";
        } else if(length == 60) {
            std::cout << length << " seconds left!\n";
        } else if(length == 45) {
            std::cout << length << " seconds left!\n";
        } else if(length == 30) {
            std::cout << length << " seconds left!\n";
            if(nextEff != ' ' && nextInt != -1)
                std::cout << "Next interval block is " << effortInterpret(nextEff) << " for " << nextInt << " seconds\n";
            else
                std::cout << "You got this! Last stretch!\n";
        } else if(length == 10) {
            std::cout << length << " seconds left!\n";
        } else if(length <= 3) {
            std::cout << length << " seconds left!\n";
        }
    }

    // Can use this to just wait for desired length
    //std::this_thread::sleep_for(totalLength);
}

// Takes in a string and delimeter to split a string into sepearte strings and outputs them into a vector
std::vector<std::string> splitString(std::string str, std::string del) {
    std::vector<std::string> vct;
    int i = 0;
    int pos = str.find(del);

    while(pos != std::string::npos) {
        vct.push_back(str.substr(0, pos));
        //std::cout << vct[i] << " ";
        str.erase(0, pos + del.length());
        pos = str.find(del);
        i++;
    }
    vct.push_back(str.substr(0, pos));
    //std::cout << vct[i] << " ";

    return vct;
}

// Reads data from file to pull saved workouts 
std::vector<data> readFile(std::string file) {
    std::vector<data> workouts;
    struct data tmpwkt;

    std::vector<std::string> v, v1; // Used to store strings from using split string
    std::string line;
    std::string del = ",";

    std::ifstream fin(file);

    if(fin.fail()) {
        std::cout << "Error opening file " << file << "\n";
        exit(1);
    }

    while(std::getline(fin, line)) {
        v = splitString(line, del);
        tmpwkt.name = v[0];
        tmpwkt.description = v[1];
        for(int i = 0; i < v[2].length(); i++)
            tmpwkt.effort.push_back(v[2][i]);
    
        v1 = splitString(v[3], "#");
        for(int j = 0; j < v1.size(); j++)
            tmpwkt.intervals.push_back(v1[j]);

        workouts.push_back(tmpwkt);
        
        //Clear the vectors out so if any other entries are shorter none of the old data pops up
        tmpwkt.effort.clear();
        tmpwkt.intervals.clear();
    }

    return workouts;
}

// Writes workout to file when user wants to save it
void writeFile(std::string file, struct data d1) {
    std::ofstream fout(file, std::ios::app);    // std::ios::app sets file up in append mode to ensure old data won't be lost

    if(fout.fail()) {
        std::cout << "Error opening file " << file << "\n";
        exit(1);
    }

    // Puts cursor at end of file and writes contents of d1 to file
    fout.seekp(0, std::ios::end);
    fout << d1.format();

    fout.close();
}

// Runs the selected workout
void doWorkout(struct data workout) {
    std::cout << "\nSelected workout. . .\n";
    workout.printData();

    char input;
    bool check = true;
    while(check) {
        std::cout << "Begin this workout? (Type Y for yes or N for no): ";
        std::cin >> input;
        switch(input) {
            case('y'):
            case('Y'):
                check = false;
                break;
            case('n'):
            case('N'):
                return;
            default:
                std::cout << "Incorrect input given, please try again\n";
                break;
        }
    }

    std::cout << "Beginning workout! . . .\n\n";
    for(int i = 0; i < workout.intervals.size(); i++) {
        std::cout << effortInterpret(workout.effort[i]) << " effort for " << secondsToMinutes(stoi(workout.intervals[i]));
        if(stoi(workout.intervals[i]) > 60) {
            std::cout << " minutes\n";
        } else {
            std::cout << " seconds\n";
        }
        if(i+1 < workout.intervals.size())
            stopwatch(stoi(workout.intervals[i]), workout.effort[i+1], stoi(workout.intervals[i+1]));
        else
            stopwatch(stoi(workout.intervals[i]));
        std::cout << "\n";
    }

    std::cout << "Workout Complete! Great Job! :D\n\n";
}

// Displays and views all current workouts found in the Saved_Workouts file
void viewWorkouts(std::vector<data> fileWorkouts) {
    for(int i = 0; i < fileWorkouts.size(); i++) {
        std::cout << "[" << i+1 << "]\n";
        fileWorkouts[i].printData();
    }

    int input;
    bool ck = true;
    while(ck) {
        std::cout << "Please select which workout you would like to do by typing the number in the the brackets above each workout (or type 0 to cancel): ";
        std::cin >> input;
        if(input > fileWorkouts.size()) {
            std::cout << "Invalid input given. Please try again!\n\n";
        } else if(input == 0) {
            return;
        } else {
            doWorkout(fileWorkouts[input-1]);
            ck = false;
        }
    }
}

// Prompts the user to create a workout and returns it as the struct data programmed at the top
struct data createWorkout(std::vector<data> fileWorkouts) {
    struct data i1;
    std::string name, desc;
    std::string interval;
    std::string effort;

    bool ck = true;
    int i;
    while(ck) {
        std::cout << "Please enter an original/unused name for the workout: ";
        std::getline(std::cin >> std::ws, name);
        for(i = 0; i < fileWorkouts.size(); i++) {
            if(fileWorkouts[i].name == name) {
                std::cout << "Repeat name given. Going back to beginning of prompt.\n";
                ck = true;
                break;
            } else
                ck = false;
        }
    }
    i1.name = name;

    std::cout << "Please enter a description for the workout: ";
    std::getline(std::cin >> std::ws, desc);
    i1.description = desc;

    std::cout << "Type E or e when ready to exit\n\n";
    for(;;) {
        std::cout << "Please input the intensity of the interval (R = recovery, B = base, P = push, A = all-out): ";
        std::cin >> effort;
        if(effort == "E" || effort == "e")
            break;
        if(effort.length() != 1) {
            std::cout << "Effort must be one character in length. Going back to beginning of prompt.\n";
            continue;
        } else if(effort != "R" && effort != "B" && effort != "P" && effort != "A" && effort != "r" && effort != "b" && effort != "p" && effort != "a") {
            std::cout << "Effort must be one of the four characters asked for. Going back to beginning of prompt.\n";
            continue;
        }

        std::cout << "Please input time of interval in seconds (whole numbers): ";
        std::cin >> interval;
        if(interval == "E" || interval == "e")
            break;
        if(!checkNumber(interval)) {
            std::cout << "Interval must be a number. Going back to beginning of prompt.\n";
            continue;
        }

        i1.effort.push_back(effort[0]);
        i1.intervals.push_back(interval);
    }

    std::cout << "\nCreating workout. . .\n";
    i1.printData();
    return i1;
}

void mainMenu(std::string file) {
    int input;
    bool ck = true;
    std::vector<data> fileWorkouts = readFile(file);
    std::cout << "Welcome to the workout interval app!\n";

    struct data d;
    while(ck) {
        std::cout << "\nPlease type the number corresponding with the option you would like to choose. . .\n";
        std::cout << "1: Select/View Workout(s)\n";
        std::cout << "2: Create Workout\n";
        std::cout << "3: Exit\n";
        std:: cin >> input;
        std::cout << "\n";
    
        switch(input) {
            case 1:
                viewWorkouts(fileWorkouts);
                break;
            case 2:
                d = createWorkout(fileWorkouts);
                writeFile(file, d);
                fileWorkouts.push_back(d);
                break;
            case 3:
                ck = false;
                break;
            default:
                std::cout << "Invalid input given. Please try again\n\n";
                break;
        }
    }

    std::cout << "Thank you for using this program. Have a great day! :D\n\n";
}

int main() {
    std::string file = "Saved_Workouts.txt";
    mainMenu(file);

    return 0;
}