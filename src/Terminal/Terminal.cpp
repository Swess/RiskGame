//
// Created by ker10 on 2019-10-22.
//

#include "Terminal.h"
#include <vector>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::to_string;

bool Terminal::debug_mode = false;
bool Terminal::test_mode = false;
int Terminal::input;

void Terminal::print(const string& s) {
    cout << s << endl;
}


void Terminal::print(const int &i) {
    cout << i << endl;
}


void Terminal::print(const std::vector<string>& array_string) {
    for (const auto & i : array_string) cout << i << endl;
}

void Terminal::debug(const string& s){
    if (debug_mode) {
        cout << "[DEBUG] ";
        print(s);
    }
}

void Terminal::error(const string &s) {
    std::cerr << "[ERROR] " << s << endl;
}

/*
* If used in test mode, make sure to define the input before use.
*/
bool Terminal::print_select(const string& s) {
    string in;
    cout << s << " (Y/N)";
    if (test_mode) {
        if (input == 0) {
            in = "N";
        } else if (input == 1){
            in = "Y";
        }
        Terminal::test(in);
    } else {
        cin >> in;
    }
    return in == "Y" || in == "y" || in == "yes" || in == "YES" || in =="ya" || in =="oui";
}

/*
* @brief If used in test mode, make sure to define the input before use.
* @output the selected index (user input - 1)
*/
int Terminal::print_select(std::vector<string> array_string) {
    int a;
    for (unsigned long long i = 1; i <= array_string.size() ; ++i) {
        cout << i << ". " << array_string.at(i-1) << endl;
    }
    if (test_mode) {
        Terminal::test(input);
        return input;
    }
    while (true) {
        cout << "Enter your selection (1-" << array_string.size() << "). " << endl;
        if ( cin >> a  && a <= array_string.size() && a > 0 ) break;
        else {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    return (a-1);
}

void Terminal::set_input(const int &s){
    input = s;
}

void Terminal::debug_mode_on() {
    debug_mode = true;
}

void Terminal::debug_mode_off() {
    debug_mode = false;
}

void Terminal::test_mode_on() {
    test_mode = true;
}

void Terminal::test_mode_off() {
    test_mode = false;
}


void Terminal::test(const string& s){
    if (test_mode) {
        cout << "[TEST] ";
        print(s);
    }
}

void Terminal::test(const int& i){
    if (test_mode) {
        cout << "[TEST] ";
        print(i);
    }
}

void Terminal::run_test(const string &s, bool (*f)()) {
    Terminal::test("Running " + s + " component tests.");
    f();
    Terminal::test(s + " component testing is successful");
}