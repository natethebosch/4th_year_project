/* 
 * File:   BlockingQueue.cpp
 * Author: Nate Bosscher
 * 
 * Created on February 24, 2016, 2:11 PM
 */

#include "BlockingQueue.h"
#include "../sys/Debug.h"
#include <xenomai/native/pipe.h>

BlockingQueue::BlockingQueue(const char* name){
    // create pipe
    int status = rt_pipe_create(&pipe, name, P_MINOR_AUTO, BLOCKING_QUEUE_DEFAULT_SIZE);
    
    // check for errors
    if(status != 0){
        switch(status){
            case ENOMEM: 
                Debug::output("Not enough memory");
                break;
            case EEXIST:
                Debug::output("Pipe with the same name already exists");
                break;
            case ENODEV:
                Debug::output("Invalid pipe minor");
                break;
            case EBUSY:
                Debug::output("Minor is already open");
                break;
            case EPERM:
                Debug::output("Called from async context");
                break;
            default:
                Debug::output("Unknown error");
                break;
        }
    }
}

BlockingQueue::~BlockingQueue(){
    rt_pipe_delete(&pipe);
    // no error detection
}

template <class T>
BlockingQueueSender<T> BlockingQueue<T>::getSender(){
    return new BlockingQueueSender<T>(&pipe);
}

template <class T>
BlockingQueueSender<T> BlockingQueue<T>::getReciever(){
    return new BlockingQueueReceiver<T>(&pipe);
}
