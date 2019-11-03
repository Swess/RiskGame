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
vector<int> Terminal::input_vector;
int Terminal::terminal_input_counter = -1;

void Terminal::print(const string& s) {
    cout << s << endl;
    std::flush(cout);
}


void Terminal::print(const int &i) {
    cout << i << endl;
    std::flush(cout);
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


void Terminal::debug(const int &i) {
    if (debug_mode) {
        cout << "[DEBUG] ";
        print(i);
    }
}


void Terminal::error(const string &s) {
    std::cerr << "[ERROR] " << s << endl;
    std::flush(std::cerr);
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


int Terminal::print_select(const int &minimum, const int &maximum, const string &s) {
    int answer;

    Terminal::print(s);

    if (test_mode) {
        Terminal::test(input);
        if ( !(input <= maximum && input >= minimum )) {
            std::cerr << "TEST INPUT WAS NOT WELL DEFINE IN THE RANGE [" << minimum << "," << maximum << "]" << endl;
        }
        return input;
    }

    while (true) {
        cout << "Choose a number between " << minimum << " and " << maximum << endl;
        if ( cin >> answer  && answer <= maximum && answer >= minimum ) break;
        else {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    std::flush(cout);

    return answer;
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
        if (terminal_input_counter != -1) {
            input = input_vector.at(terminal_input_counter);
            terminal_input_counter++;
        }
        Terminal::test(input + 1);
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
    std::flush(cout);
    return (a-1);
}

void Terminal::set_input(const int &s){
    input = s;
}

void Terminal::set_input_vector(const vector<int> & vector) {
    terminal_input_counter = 0;
    input_vector = vector;
}

void Terminal::clear_terminal_input_counter() {
    terminal_input_counter = -1;
}

int Terminal::get_input(){
    return input;
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

void Terminal::print_on_same_line(const std::vector<string> &array_string) {
    for (const auto& element: array_string){
        cout << element << " ";
    }
    cout << endl;
}

void Terminal::print_on_same_line(const std::vector<int> &array_int) {
    for (const auto& element: array_int){
        cout << element << " ";
    }
    cout << endl;
}

