/*
 * File:   TBlockingQueue.h
 * Author: Nate Bosscher
 *
 * Created on February 26, 2016, 8:35 PM
 */

#include <iostream>

#include <sstream>
#include <stdlib.h>

#include "../src/helpers/BlockingQueue.h"
#include "./Test.h"

using namespace std;

class TBlockingQueue : public Test{
public:
    
    bool test(){
        
        BlockingQueue<int> bq("Test-1");
        BlockingQueueReceiver<int> *bqr = bq.getReceiver();
        
        // test receiver timeout
        try{
            bqr->take(1000);
        }catch(BlockingQueueStatus status){
            if(status != BLOCKING_QUEUE_TIMEOUT){
                ostringstream stringStream;
                stringStream << "should have timed out. Instead threw " << status;
                error = stringStream.str();
                
                return false;
            }
        }
        
        BlockingQueueSender<int> *bqs = bq.getSender();
        
        int send = rand();
        
        try{
            bqs->put(&send);
        }catch(BlockingQueueStatus status){
            ostringstream stringStream;
            stringStream << "should have sent value. Instead threw " << status;
            error = stringStream.str();
            return false;
        }
        
        int value;
        
        try{
            value = bqr->take(1000);
        }catch(BlockingQueueStatus status){
            ostringstream stringStream;
            stringStream << "should have got value. Instead threw " << status;
            error = stringStream.str();
            return false;
        }
        
        if(value != send){
            ostringstream stringStream;
            stringStream << "should have got " << value << ". Instead got " << send;
            error = stringStream.str();
            
            return false;
        }
        
        return true;

    }
};