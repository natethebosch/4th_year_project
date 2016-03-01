//
//  Mutex.cpp
//  4th_year_project_r2
//
//  Created by nate bosscher on 2016-03-01.
//  Copyright © 2016 nate bosscher. All rights reserved.
//

#include "Mutex.h"


bool Mutex::lock(RTIME timeout){
    int status = rt_mutex_acquire(&mutex, timeout);
    
    switch(status){
        case 0:
            return true;
        case EINVAL:
            Debug::output("mutex is not a mutex descriptor.");
            break;
        case EIDRM:
            Debug::output("mutex is a deleted mutex descriptor, including if the deletion occurred while the caller was sleeping on it.");
            break;
        case EWOULDBLOCK:
            Debug::output("timeout is equal to TM_NONBLOCK and the mutex is not immediately available.");
            break;
        case EINTR:
            Debug::output("rt_task_unblock() has been called for the waiting task before the mutex has become available.");
            break;
        case ETIMEDOUT:
            Debug::output("the mutex cannot be made available to the calling task within the specified amount of time.");
            break;
        case EPERM:
            Debug::output("this service was called from a context which cannot be given the ownership of the mutex (e.g. interrupt, non-realtime context).");
            break;
        default:
            Debug::output("Unknown error");
            break;
    }
    
    return false;
}

void Mutex::release(){
    int status = rt_mutex_release(&mutex);
    switch(status){
        case 0: return;
        case EINVAL:
        case EIDRM:
        case EPERM:
    }
}