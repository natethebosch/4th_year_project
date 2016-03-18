//
//  Mutex.cpp
//  4th_year_project_r2
//
//  Created by nate bosscher on 2016-03-01.
//  Copyright © 2016 nate bosscher. All rights reserved.
//

#include "Mutex.h"

// init static variable
int Mutex::mutex_count = 0;

Mutex::Mutex(){
    // make string of format mut_##
    std::string st("mut_");
    st += std::to_string(Mutex::mutex_count++);
    
    // create mutex and display messages according to status flag
    int status = rt_mutex_create(&mutex, st.c_str());
    
    switch(status){
        case 0:
            return;
        case -ENOMEM:
            Debug::output("Mutext::Mutex - the system fails to get enough dynamic memory from the global real-time heap in order to register the mutex.");
            break;
        case -EEXIST:
            Debug::output("Mutext::Mutex - the name is already in use by some registered object.");
            break;
        case -EPERM:
            Debug::output("Mutext::Mutex - this service was called from an asynchronous context.");
            break;
        default:
            Debug::output("Mutext::Mutex - Unknown error");
            break;
    }
}


bool Mutex::lock(RTIME timeout){
    int status = rt_mutex_acquire(&mutex, timeout);
    
    switch(status){
        case 0:
            return true;
        case -EINVAL:
            Debug::output("Mutext::lock - mutex is not a mutex descriptor.");
            break;
        case -EIDRM:
            Debug::output("Mutext::lock - mutex is a deleted mutex descriptor, including if the deletion occurred while the caller was sleeping on it.");
            break;
        case -EWOULDBLOCK:
            Debug::output("Mutext::lock - timeout is equal to TM_NONBLOCK and the mutex is not immediately available.");
            break;
        case -EINTR:
            Debug::output("Mutext::lock - rt_task_unblock() has been called for the waiting task before the mutex has become available.");
            break;
        case -ETIMEDOUT:
            Debug::output("Mutext::lock - the mutex cannot be made available to the calling task within the specified amount of time.");
            break;
        case -EPERM:
            Debug::output("Mutext::lock -  this service was called from a context which cannot be given the ownership of the mutex (e.g. interrupt, non-realtime context).");
            break;
        default:
            Debug::output("Mutext::lock - Unknown error");
            break;
    }
    
    return false;
}

void Mutex::release(){
    int status = rt_mutex_release(&mutex);
    switch(status){
        case 0: return;
        case -EINVAL:
        case -EIDRM:
        case -EPERM:
            break;
    }
    
    return;
}