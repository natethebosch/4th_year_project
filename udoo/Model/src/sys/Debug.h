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
    static void output(const char* msg, bool skipDebugInfo){
        if(DEBUG && !skipDebugInfo){
            std::cout << "DEBUG: " << msg << "\n";
            std::cout << "... jumping to stacktrace...\n";
            std::cout.flush();
//            printSomething();
//            std::cout << Backtrace();
            std::cout << "done stacktrace...\n";
            std::cout.flush();
        }else{
            std::cout << "DEBUG: " << msg << "\n";
        }
    }
    
    static void output(const char* msg){
        output(msg, true);
    }
private:

};

#endif /* DEBUG_H */

