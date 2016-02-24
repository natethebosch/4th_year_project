/* 
 * File:   BlockingQueue.cpp
 * Author: Nate Bosscher
 * 
 * Created on February 24, 2016, 2:11 PM
 */

#include "BlockingQueue.h"
#include "Debug.h"
#include <native/pipe.h>

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

BlockingQueueReceiver<T> BlockingQueue::getReceiver(){
    return new BlockingQueueReceiver<T>(&pipe);
}

BlockingQueueSender<T> BlockingQueue::getSender(){
    return new BlockingQueueSender<T>(&pipe);
}

T BlockingQueueReceiver::take() throw (BlockingQueueStatus){
    return take(TM_INFINITE);
}

T BlockingQueueReceiver::take(size_t timeout) throw (BlockingQueueStatus){
    RT_PIPE_MSG* msg;
    
    // wait indefinitely for incomming message
    ssize_t status = rt_pipe_receive(pipe, &msg, timeout);
    
    // check for errors
    if(status != P_MSGSIZE(msg))
        switch(status){
            case 0:
                Debug::output("Pipe closed during receiving");
                break;
            case EINVAL:
                Debug::output("Pipe is not a pipe descriptor");
                break;
            case ENODEV:
            case EBADF:
                Debug::output("Pipe is scrambled");
                break;
            case ETIMEDOUT: // all are different errors, but group together to represent timeout
            case EWOULDBLOCK:
            case EINTR:
                Debug::output("Timeout Interrupted");
                throw BLOCKING_QUEUE_TIMEOUT;
                break;
            case EPERM:
                Debug::output("this service should block, but was called from a context which cannot sleep");
                break;
        }
        
        throw BLOCKING_QUEUE_FAIL_RECV;
    }
    
    T output;
    output = *((T*)P_MSGPTR(msg));
    rt_pipe_free(msg);
    
    return output;
}

void BlockingQueueSender::put(T* item) throw (BlockingQueueStatus){
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