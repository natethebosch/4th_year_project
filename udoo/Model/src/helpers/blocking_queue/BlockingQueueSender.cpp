/* 
 * File:   BlockingQueueSender.cpp
 * Author: Nate Bosscher
 * 
 * Created on February 24, 2016, 3:35 PM
 */

#include "BlockingQueueSender.h"

template <class T>
void BlockingQueueSender<T>::put(T* item) throw (BlockingQueueStatus){
    RT_PIPE_MSG* msg; 
    
    // allocate a message
    msg = rt_pipe_alloc(sizeof(T)); // will be automatically freed
    if(!msg){
        throw BLOCKING_QUEUE_NO_ALLOC;
    }
    
    // copy item to message
    memcpy(RT_PIPE_MSGPTR(msg), item, sizeof(T));
    
    // perform send
    ssize_t status = rt_pipe_send(pipe, msg, sizeof(T), P_NORMAL);
    
    // check for errors
    switch(status){
        case EINVAL:
            Debug::output("Invalid pipe");
            break;
        case EPIPE:
            Debug::output("Pipe is not yet open");
            break;
        case EIDRM:
            Debug::output("Pipe is closed");
            break;
        case ENODEV:
        case EBADF:
            Debug::output("Pipe is scrambled");
            break;
        default:
            // success
            break;
    }
}

