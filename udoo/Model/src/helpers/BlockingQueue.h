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

// xenomai
#include <queue.h>

#include "./blocking_queue/BlockingQueueStatus.h"
#include "../sys/Debug.h"

#include "../type/SensorDataPoint.h"

#ifndef BLOCKINGQUEUE_H
#define BLOCKINGQUEUE_H

#define BQ_SIZE 1000 // 1000 items of type T


template <typename T>
class BlockingQueue {
private:
    RT_QUEUE queue;
    
public:
    /**
     * instantiates a BlockingQueue of default size
     */
    BlockingQueue(const char* name){
        BlockingQueue(name, BQ_SIZE);
    }

    /**
     * instantiates a BlockingQueue to fit <count> items of type T
     */
    BlockingQueue(const char* name, size_t count){
        
        // create pipe
        int status;
        if(sizeof(T) < 32){
            status = rt_queue_create(&queue, name, count * 32, Q_UNLIMITED, Q_FIFO);
        }else{
            status = rt_queue_create(&queue, name, count * sizeof(T), Q_UNLIMITED, Q_FIFO);
        }
        
        // check for errors
        if(status != 0){
            switch(status){
                case -ENOMEM: Debug::output("ENOMEM"); break;
                case -EEXIST: Debug::output("EEXIST"); break;
                case -EINVAL: Debug::output("EINVAL"); break;
                case -EPERM: Debug::output("EPERM"); break;
                case -ENOSYS: Debug::output("ENOSYS"); break;
                case -ENOENT: Debug::output("ENOENT"); break;
                default:
                    Debug::output("Unknown error");
                    break;
            }
        }
    }
    
    ~BlockingQueue<T>(){
        rt_queue_delete(&this->queue);
        // no error detection
    }
    
    BlockingQueue<T>& operator =(const BlockingQueue<T>& rhs) {
        queue = rhs.queue;
        return this;
    }
    
    /**
     * take an item from the queue
     * throws BQ_TIMEOUT if timeout is reached first
     */
    T take(RTIME timeout) throw (BlockingQueueStatus){
        // alocate memory for type T
        void* buff;
        
        ssize_t status = -EINTR;
        size_t maxct = 100;
        size_t ct = 0;
        
        // continue looping if interrupted by thread
        while(status == -EINTR && ct < maxct){
            if(ct != 0 && status == -EINTR){
                Debug::output("EINTR");
            }
            
            // read from queue or timeout
            status = rt_queue_receive(&queue, &buff, timeout);
            ct++;
        }
        
        // if there's a failure, free the memory before throwing error
        if(status <= 0){
            rt_queue_free(&queue, buff);
        }
        
        // decode status for errors
        switch(status){
            case -ETIMEDOUT:
//                Debug::output("ETIMEDOUT");
                throw BQ_TIMEOUT;
                break;
            case -EWOULDBLOCK:
                Debug::output("EWOULDBLOCK");
                throw BQ_FAIL;
                break;
            case -EINTR:
                Debug::output("EINTR -- maxct exceeded");
                throw BQ_FAIL;
                break;
            case -EINVAL:
                Debug::output("EINVAL");
                throw BQ_FAIL;
                break;
            case -EIDRM:
                Debug::output("EIDRM");
                throw BQ_FAIL;
                break;
            case -EPERM:
                Debug::output("EPERM");
                throw BQ_FAIL;
                break;
            default:
                // nothing to do
                break;
        }
        
        // copy to non-queue-managed portion of memory
        T output;
        memcpy(&output, buff, sizeof(T));
        
        // free the queue memory
        rt_queue_free(&queue, buff);
        
        return output;
    }
    
    /**
     * block until an item is available
     */
    T take() throw (BlockingQueueStatus) {
        return take(TM_INFINITE);
    }
    
    /**
     * adds an item to the queue
     */
    void put(T item) throw (BlockingQueueStatus){
        
        // allocate message
        void* buff = rt_queue_alloc(&queue, sizeof(T));
        
        // copy to message buffer
        memcpy(buff, &item, sizeof(T));
        
        // send
        int status = rt_queue_send(&queue, buff, sizeof(item), Q_NORMAL);
        switch(status){
            case 0:
                break;
            case -EINVAL: Debug::output("EINVAL"); throw BQ_FAIL; break;
            case -ENOMEM: Debug::output("ENOMEM"); throw BQ_FAIL; break;
        }
    }
    
    /**
     * prints current queue usage and queue buffer size
     */
    void info(){
        RT_QUEUE_INFO info;
        rt_queue_inquire(&queue, &info);
        
        printf("Queue Info:\n pool size: %u\n used: %u\n\n", info.poolsize, info.usedmem);
    }
    
};

#endif /* BLOCKINGQUEUE_H */

