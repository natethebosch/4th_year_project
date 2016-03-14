/* 
 * File:   Debug.h
 * Author: Nate Bosscher
 *
 * Created on February 24, 2016, 12:59 PM
 */

#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include "../constants.h"
#include "../helpers/stacktrace.h"

class Debug {
public:
    static void output(const char* msg){
        if(DEBUG){
            std::cout << "DEBUG: " << msg << "\n";
            std::cout << "... jumping to stacktrace...\n";
            std::cout.flush();
//            printSomething();
            std::cout << Backtrace();
            std::cout << "done stacktrace...\n";
            std::cout.flush();
        }
    }
private:

};

#endif /* DEBUG_H */

