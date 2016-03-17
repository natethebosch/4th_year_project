/* 
 * File:   BlockingQueue.cpp
 * Author: Nate Bosscher
 * 
 * Created on February 24, 2016, 2:11 PM
 */

#include "BlockingQueue.h"

//template<>
//void BlockingQueue<SensorDataPoint>::put(SensorDataPoint item) throw (BlockingQueueStatus){
//    // allocate message
//    void* buff = rt_queue_alloc(&queue, sizeof(SensorDataPoint));
//    
//    printf("INSERT: %f %f %f\n", item.x, item.y, item.value);
//    
//    // copy to message buffer
//    memcpy(buff, &item, sizeof(SensorDataPoint));
//    
//    // send
//    int status = rt_queue_send(&queue, buff, sizeof(item), Q_NORMAL);
//    switch(status){
//        case 0:
//            break;
//        case -EINVAL: Debug::output("EINVAL"); throw BQ_FAIL; break;
//        case -ENOMEM: Debug::output("ENOMEM"); throw BQ_FAIL; break;
//    }
//}

/*

int 0, 32, 64, 96,

*/