/*
 * File:   Test.h
 * Author: Nate Bosscher
 *
 * Created on February 26, 2016, 8:35 PM
 */

#include <iostream>

class Test{
public:
    std::string error;
    
    virtual bool test(){
        std::cout << "No test method has been defined";
    }
};