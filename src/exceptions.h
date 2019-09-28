//
// Created by ker10 on 2019-09-28.
//

#include <exception>

// Exceptions
struct IOException : public std::exception {
    const char *what() const throw() {
        return "Error opening given file: ";
    }
};
