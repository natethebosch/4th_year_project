/* 
 * File:   BlockingQueueSender.h
 * Author: Nate Bosscher
 *
 * Created on February 24, 2016, 3:35 PM
 */

#ifndef BLOCKINGQUEUESENDER_H
#define BLOCKINGQUEUESENDER_H

#include "BlockingQueueStatus.h"
#include "../../sys/Debug.h"

#include <xenomai/native/queue.h>

template <class T>
class BlockingQueueSender{
private:
    RT_QUEUE *queue;
public:
    
    BlockingQueueSender(RT_QUEUE *_queue){
        queue = _queue;
    }
    
    /**
     * adds an item to the end of the queue
     * @param item
     */
    void put(T* item) throw (BlockingQueueStatus){
        void* msg;
        
        // allocate a message
        msg = rt_queue_alloc(this->queue, sizeof(T)); // will be automatically freed
        if(!msg){
            throw BLOCKING_QUEUE_NO_ALLOC;
        }
        
        // copy item to message
        memcpy(msg, item, sizeof(T));
        
        // perform send
        ssize_t status = rt_queue_send(this->queue, msg, sizeof(T), Q_NORMAL);
        
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
    
    RT_QUEUE* getQueue(){
        return queue;
    }
};

#endif /* BLOCKINGQUEUESENDER_H */

