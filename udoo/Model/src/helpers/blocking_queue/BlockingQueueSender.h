/* 
 * File:   BlockingQueueSender.h
 * Author: Nate Bosscher
 *
 * Created on February 24, 2016, 3:35 PM
 */

#ifndef BLOCKINGQUEUESENDER_H
#define BLOCKINGQUEUESENDER_H

#include "BlockingQueueStatus.h"

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
    void put(T item) throw (BlockingQueueStatus);
};

#endif /* BLOCKINGQUEUESENDER_H */

