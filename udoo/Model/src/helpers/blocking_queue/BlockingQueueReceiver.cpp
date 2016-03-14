/* 
 * File:   BlockingQueueReceiver.cpp
 * Author: Nate Bosscher
 * 
 * Created on February 24, 2016, 3:33 PM
 */

#include "BlockingQueueReceiver.h"

template <class T>
T BlockingQueueReciever::take(size_t timeout) throw (BlockingQueueStatus){
        void* msg;
        
        // wait indefinitely for incomming message
        ssize_t status = rt_queue_receive(queue, &msg, timeout);
        
        // check for errors
        switch(status){
            case 0:
                Debug::output("Pipe closed during receiving");
                throw BLOCKING_QUEUE_FAIL_RECV;
                break;
            case EINVAL:
                Debug::output("Pipe is not a pipe descriptor");
                throw BLOCKING_QUEUE_FAIL_RECV;
                break;
            case ENODEV:
            case EBADF:
                Debug::output("Pipe is scrambled");
                throw BLOCKING_QUEUE_FAIL_RECV;
                break;
            case ETIMEDOUT: // all are different errors, but group together to represent timeout
            case EWOULDBLOCK:
            case EINTR:
                Debug::output("Timeout Interrupted");
                throw BLOCKING_QUEUE_TIMEOUT;
                break;
            case EPERM:
                Debug::output("this service should block, but was called from a context which cannot sleep");
                throw BLOCKING_QUEUE_FAIL_RECV;
                break;
            default:
                break;
        }
        
        T output;
        output = *((T*)msg);
        
        rt_queue_free(queue, msg);
        
        return output;
    }
