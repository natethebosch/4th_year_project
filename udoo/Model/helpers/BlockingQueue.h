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
#include <native/pipe.h>

#ifndef BLOCKINGQUEUE_H
#define BLOCKINGQUEUE_H

#define BLOCKING_QUEUE_DEFAULT_SIZE 10

typedef enum BlockingQueueStatus{
    BLOCKING_QUEUE_NO_ALLOC, BLOCKING_QUEUE_TIMEOUT, BLOCKING_QUEUE_FAIL_RECV
} BlockingQueueStatus;

template <class T>
class BlockingQueue {
private:
    RT_PIPE pipe;
    
public:
    BlockingQueue(const char* name);
    ~BlockingQueue();
    
    BlockingQueueReceiver<T> getReceiver();
    BlockingQueueSender<T> getSender();
};

template <class T>
class BlockingQueueReceiver{
private:
    RT_PIPE *pipe;
public:
    BlockingQueueReceiver(RT_PIPE *_pipe){
        pipe = _pipe;
    }
    
    /**
     * waits forever until there's a new item in the queue
     * @return 
     */
    T take() throw (int);
    T take(size_t timeout) throw (int);
};

template <class T>
class BlockingQueueSender{
private:
    RT_PIPE *pipe;
public:
    
    BlockingQueueSender(RT_PIPE *_pipe){
        pipe = _pipe;
    }
    /**
     * adds an item to the end of the queue
     * @param item
     */
    void put(T item) throw (int);
};

#endif /* BLOCKINGQUEUE_H */

