/* 
 * File:   BlockingQueueReceiver.cpp
 * Author: Nate Bosscher
 * 
 * Created on February 24, 2016, 3:33 PM
 */

#include "BlockingQueueReceiver.h"

BlockingQueueReceiver<T> BlockingQueue::getReceiver(){
    return new BlockingQueueReceiver<T>(&pipe);
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
