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

#define BLOCKING_QUEUE_DEFAULT_SIZE 10


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

#endif /* BLOCKINGQUEUE_H */

