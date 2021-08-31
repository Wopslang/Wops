/*
 * src/error/signal.h
 * Wopslang Error Signal Headerfile
 *
 * 2021, Wops Team
 * */

#ifndef WOPS_ERRORSIGNAL_H
#define WOPS_ERRORSIGNAL_H

#include <iostream>

// enum Err {OK, ERROR}
// Enumeration of error types
enum Err {
    OK,
    ERROR,
    // :TODO
};

class ErrHandler {
    public:
    void CallErr(std::string errmsg) {
        std::cout << "\e[31m" << errmsg << "\e[m";
        exit(1);
        return;
    }
};

#endif
