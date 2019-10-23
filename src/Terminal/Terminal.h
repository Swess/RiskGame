//
// Created by ker10 on 2019-10-22.
//

#ifndef TEAM12_DOMINATIONGAME_TERMINAL_H
#define TEAM12_DOMINATIONGAME_TERMINAL_H

#include <string>
#include <vector>

using std::string;

class Terminal {
    public:

    static void print(const string& s);
    static void debug(const string& s);
    static void print(const std::vector<string>& array_string);
    static bool print_select(const string& s);
    static int print_select(std::vector<string> s);
    static void debug_mode_on();
    static void test_mode_on();
    static void test(const string &s);
    static void error(const string &s);
    static void run_test( const string& s,  bool (*f)() );
private:
    static bool debug_mode;
    static bool test_mode;
};


#endif //TEAM12_DOMINATIONGAME_TERMINAL_H
