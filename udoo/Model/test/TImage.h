
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

// xenomai
#include "timer.h"

#include "./Test.h"
#include "../src/tasks/ImageProcessor.h"

class ImageTest: public Test {
public:
    /**
     * tests one image
     */
    bool test1(){
        SensorDataPoint* dp;
        BlockingQueue<SensorDataPoint>* queue;
        
        queue = new BlockingQueue<SensorDataPoint>("Serial2ImageProcessorQueue", 24*24);
        
        ImageProcessor *imgProcessor= new ImageProcessor(queue, "/root/model/test/ws-test");
        imgProcessor->start();
        dp=(SensorDataPoint*)malloc(sizeof(SensorDataPoint));
        
        
        for (int i=0; i<24; i++){
            for (int n=0;n<24; n++){
                
                dp->y = i*40+1;
                dp->x = n*20;
                
                // try to get a nice range between 0..1023
                dp->value = (i*n*20) % 1024;
                queue->put(*dp);
            }
        }
        
        dp->y=0;
        dp->x=0;
        dp->value=0;
        queue->put(*dp);
        
        free(dp);
        
        //imgProcessor->kill();
        
        return true;
    }
    
    /**
     * tests multiple images
     */
    bool test2(){
        SensorDataPoint* dp;
        BlockingQueue<SensorDataPoint>* queue;
        
        queue = new BlockingQueue<SensorDataPoint>("Serial2ImageProcessorQueue", 24*24*15);
        
        ImageProcessor *imgProcessor = new ImageProcessor(queue, "/root/model/test/ws-test");
        imgProcessor->start();
        
        dp=(SensorDataPoint*)malloc(sizeof(SensorDataPoint));
        
        
        // create multiple images
        
        for(int j = 0; j < 15; j++){
            for (int i=0; i<24; i++){
                for (int n=0;n<24; n++){
                    
                    dp->y = i*40;
                    dp->x = n*20;
                    
                    // try to get a nice range between 0..1023
                    dp->value = (i*n*j) % 1024;
                    queue->put(*dp);
                }
            }
            
            std::cout << "Sleeping for 1 sec...\n";
            rt_task_sleep(rt_timer_ns2ticks(1000000000));
            std::cout << "Awake!\n";
        }
        
        dp->y=0;
        dp->x=0;
        dp->value=0;
        queue->put(*dp);
        
        free(dp);
        
        return true;
    }
    
    bool test()
    {
        return test2();
//        return test1() && test2();
    }
};
