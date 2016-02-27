/* 
 * File:   BlockingQueue.h
 * Author: Nate Bosscher
 *
 * Created on February 24, 2016, 2:11 PM
 */

#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <xenomai/native/pipe.h>

#ifndef BLOCKINGQUEUE_H
#define BLOCKINGQUEUE_H

#include "./blocking_queue/BlockingQueueReceiver.h"
#include "./blocking_queue/BlockingQueueSender.h"
#include "./blocking_queue/BlockingQueueStatus.h"
#include "../sys/Debug.h"

#define BLOCKING_QUEUE_DEFAULT_SIZE 10


template <class T>
class BlockingQueue {
private:
    RT_QUEUE queue;
    
public:
    BlockingQueue(const char* name){
        // create pipe
        int status = rt_queue_create(&this->queue, name, BLOCKING_QUEUE_DEFAULT_SIZE * 8, BLOCKING_QUEUE_DEFAULT_SIZE, Q_FIFO);
        
        // check for errors
        if(status != 0){
            switch(status){
                case ENOMEM:
                    Debug::output("Not enough memory");
                    break;
                case EEXIST:
                    Debug::output("Pipe with the same name already exists");
                    break;
                case EINVAL:
                    Debug::output(" poolsize is null, greater than the system limit, or name is null or empty for a shared queue.");
                    break;
                case EPERM:
                    Debug::output("this service was called from an invalid context");
                    break;
                case ENOSYS:
                    Debug::output("mode specifies Q_SHARED, but the real-time support in user-space is unavailable");
                    break;
                case ENOENT:
                    Debug::output("/dev/rtheap can't be opened.");
                    break;
                default:
                    Debug::output("Unknown error");
                    break;
            }
        }
    }
    
    ~BlockingQueue(){
        rt_queue_delete(&this->queue);
        // no error detection
    }
    
    BlockingQueueReceiver<T>* getReceiver(){
        return new BlockingQueueReceiver<T>(&this->queue);
    }
    
    BlockingQueueSender<T>* getSender(){
        return new BlockingQueueSender<T>(&this->queue);
    }
    
};

#endif /* BLOCKINGQUEUE_H */

