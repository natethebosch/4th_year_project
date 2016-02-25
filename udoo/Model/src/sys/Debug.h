/* 
 * File:   Debug.h
 * Author: Nate Bosscher
 *
 * Created on February 24, 2016, 12:59 PM
 */

#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>

class Debug {
public:
    static void output(const char* msg){
        if(DEBUG){
            std::cout "DEBUG: " << msg;
            std::cout.flush();
        }
    }
private:

};

#endif /* DEBUG_H */

