//
//  Mutex.hpp
//  4th_year_project_r2
//
//  Created by nate bosscher on 2016-03-01.
//  Copyright © 2016 nate bosscher. All rights reserved.
//

#ifndef Mutex_hpp
#define Mutex_hpp

#include <stdio.h>

#include <string>

#include <iostream>
#include <native/mutex.h>
#include "Debug.h"

class Mutex{
    RT_MUTEX mutex;
    static int mutex_count;
public:
    Mutex(){
        
        // make string of format mut_##
        std::string st("mut_");
        st += std::to_string(mutex_count++);
        
        // create mutex and display messages according to status flag
        int status = rt_mutex_create(&mutex, st.c_str());
        
        switch(status){
            case 0:
                return;
            case ENOMEM:
                Debug::output("the system fails to get enough dynamic memory from the global real-time heap in order to register the mutex.");
                break;
            case EEXIST:
                Debug::output("the name is already in use by some registered object.");
                break;
            case EPERM:
                Debug::output("this service was called from an asynchronous context.");
                break;
            default:
                Debug::output("Unknown error");
                break;
        }
    }
    
    ~Mutex(){
        int status = rt_mutex_delete(&mutex);
        
        switch(status){
            case 0:
                return;
            case EINVAL:
                Debug::output("mutex is not a mutex descriptor.");
                break;
            case EIDRM:
                Debug::output("mutex is a deleted mutex descriptor.");
                break;
            case EPERM:
                Debug::output("this service was called from an asynchronous context.");
                break;
            default:
                Debug::output("Unknown error");
                break;
        }
    }
    
    bool lock(RTIME timeout);
    bool lock(){
        return lock(TM_INFINITE);
    }
    
    void release();
};

#endif /* Mutex_hpp */
