/* 
 * File:   BlockingQueueStatus.h
 * Author: Nate Bosscher
 *
 * Created on February 24, 2016, 3:36 PM
 */

#ifndef BLOCKINGQUEUESTATUS_H
#define BLOCKINGQUEUESTATUS_H


typedef enum BlockingQueueStatus{
    BLOCKING_QUEUE_NO_ALLOC, BLOCKING_QUEUE_TIMEOUT, BLOCKING_QUEUE_FAIL_RECV
} BlockingQueueStatus;


#endif /* BLOCKINGQUEUESTATUS_H */

