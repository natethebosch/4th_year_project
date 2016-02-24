/* 
 * File:   BlockingQueueReceiver.h
 * Author: Nate Bosscher
 *
 * Created on February 24, 2016, 3:33 PM
 */

#ifndef BLOCKINGQUEUERECEIVER_H
#define BLOCKINGQUEUERECEIVER_H

#include "BlockingQueueStatus.h"

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

#endif /* BLOCKINGQUEUERECEIVER_H */

