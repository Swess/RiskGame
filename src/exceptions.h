//
// Created by ker10 on 2019-09-28.
//

#include <exception>

// Exceptions
struct IOException : public std::exception {
    const char *what(string s) const throw() {
        string what = "Error opening given file: "  + s;
        return what.c_str();
    }
};
