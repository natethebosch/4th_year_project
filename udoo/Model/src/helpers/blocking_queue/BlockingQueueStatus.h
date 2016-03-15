/* 
 * File:   BlockingQueueStatus.h
 * Author: Nate Bosscher
 *
 * Created on February 24, 2016, 3:36 PM
 */

#ifndef BLOCKINGQUEUESTATUS_H
#define BLOCKINGQUEUESTATUS_H


typedef enum BlockingQueueStatus{
    BQ_TIMEOUT,
    BQ_FAIL,
    BQ_UNKNOWN
} BlockingQueueStatus;


#endif /* BLOCKINGQUEUESTATUS_H */

