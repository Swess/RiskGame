//
// Created by ker10 on 2019-10-22.
//

#ifndef TEAM12_DOMINATIONGAME_TERMINAL_H
#define TEAM12_DOMINATIONGAME_TERMINAL_H

#include <string>
#include <vector>

using std::string;
using std::vector;

class Terminal {
    public:

    static void print(const string& s);
    static void print(const int& i);
    static void debug(const string& s);
    static void debug(const int& i);
    static void print(const std::vector<string>& array_string);
    static bool print_select(const string& s);
    static int print_select(std::vector<string> s);
    static void debug_mode_on();
    static void debug_mode_off();
    static void error(const string &s);
    static void test_mode_on();
    static void test_mode_off();
    static void test(const string &s);
    static void test(const int &i);
    static void set_input(const int &s);
    static void set_input_vector(const vector<int> &vector);
    static void clear_terminal_input_counter();
    static void run_test( const string& s,  bool (*f)() );
private:
    static bool debug_mode;
    static bool test_mode;
    static int input;
    static vector<int> input_vector;
    static int terminal_input_counter;

};


#endif //TEAM12_DOMINATIONGAME_TERMINAL_H
