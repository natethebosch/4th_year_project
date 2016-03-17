/*
 * File:   Test.h
 * Author: Nate Bosscher
 *
 * Created on February 26, 2016, 8:35 PM
 */


#ifndef __TESTING_H__
#define __TESTING_H__

#include <iostream>
#include <stdlib.h>

class Test{
public:
    std::string error;
    int testSeg;
    
    Test(){
        testSeg = 0;
        error = "";
    }
    
    virtual bool test(){
        std::cout << "No test method has been defined";
        return false;
    }
    
    /**
     * prints out the test segment number for debugging
     */
    void testSegment(){
        std::cout << "\tTest segment " << testSeg << "\n";
        std::cout.flush();
        testSeg++;
    }
};

#endif //__TESTING_H__
