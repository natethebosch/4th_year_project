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

// xenomai
#include <mutex.h>

#include "Debug.h"

class Mutex{
    RT_MUTEX mutex;
public:
    static int mutex_count;
    
    Mutex();
    
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
