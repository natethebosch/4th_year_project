/*
 * File:   TBlockingQueue.h
 * Author: Nate Bosscher
 *
 * Created on February 26, 2016, 8:35 PM
 */

#include <iostream>

#include <sstream>
#include <stdlib.h>

// xenomai
#include <task.h>

#include "../src/helpers/BlockingQueue.h"
#include "./Test.h"

using namespace std;

void addRandomIntToQueue(void *arg){
    printf("Started addRandomIntToQueue\n");
    printf("yeild...\n");
    
    // allow other task to continue
    rt_task_yield();
    
    printf("continue\n");
    
    printf("finished add\n");
}

class TBlockingQueue : public Test{
public:
    
    bool test(){
        BlockingQueue<int> _bq("Test-1", 1000);
        BlockingQueue<int> *bq = &_bq;
        
        // update status
        testSegment();
        
        // test receiver timeout
        try{
            bq->take(1);
        }catch(BlockingQueueStatus status){
            if(status != BQ_TIMEOUT){
                ostringstream stringStream;
                stringStream << "should have timed out. Instead threw " << status;
                error = stringStream.str();
                
                return false;
            }
        }
        
        // update status
        testSegment();
        
        int send = rand();
        
        try{
            bq->put(send);
        }catch(BlockingQueueStatus status){
            ostringstream stringStream;
            cout << "should have sent value. Instead threw " << status;
            cout.flush();
            error = stringStream.str();
            return false;
        }
        
        int value;
        
        try{
            value = bq->take();
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
        
        
        
        std::cout << "Testing for allocation errors. addint 1000 ints. should segfault if not cleaned properly\n";
        std::cout.flush();
        
        try{
            printf("\n");
            for(int i = 0; i < 1000; i++){
                printf("At %d\n", i);
                bq->put(send);
            }
        }catch(BlockingQueueStatus status){
            ostringstream stringStream;
            cout << "should have sent value. Instead threw " << status;
            cout.flush();
            error = stringStream.str();
            return false;
        }
        
        std::cout << "Success!\n";
        
        
        return true;

    }
};