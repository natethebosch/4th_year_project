/* 
 * File:   BlockingQueueReceiver.h
 * Author: Nate Bosscher
 *
 * Created on February 24, 2016, 3:33 PM
 */

#ifndef BLOCKINGQUEUERECEIVER_H
#define BLOCKINGQUEUERECEIVER_H

#include "BlockingQueueStatus.h"
#include "../../sys/Debug.h"

// xenomai
#include <queue.h>
#include <timer.h>


class BlockingQueueReceiver{
protected:
    RT_QUEUE *queue;
public:
    BlockingQueueReceiver(RT_QUEUE *_queue){
        queue = _queue;
    }
    
    /**
     * waits forever until there's a new item in the queue
     * @return 
     */
    T take() throw (BlockingQueueStatus){
        return take(TM_INFINITE);
    }
    
    T take(size_t timeout) throw (BlockingQueueStatus);
    
    RT_QUEUE* getQueue(){
        return queue;
    }
};

#endif /* BLOCKINGQUEUERECEIVER_H */

