/**
 * Project Realtime Solar Scanner Control
 * @author Nate Bosscher, Tyler d'Entremont
 * @version 1.0
 */


#ifndef _IMAGEPROCESSOR_H
#define _IMAGEPROCESSOR_H

#include "../sys/Task.h"
#include "../helpers/BlockingQueue.h"
#include "../type/SensorDataPoint.h";
#include "ImageProcessorHeader"


class ImageProcessor: public Task {
    BlockingQueueReceiver<SensorDataPoint> input;
    
public:
    ImageProcessor(BlockingQueueReceiver<SensorDataPoint> _input): Task("ImageProcessor", 20){
        input = _input;
    };
    
    void run(void* argc){
        // image processor main logic goes here
      
          SensorDataPoint dp;
          
          // infinite loop
          for(;;){ 
               // fetch from the buffer
         
               try{
                  dp = input.take();
               }catch(BlockingQueueStatus s){
                   if(s == BLOCKING_QUEUE_TIMEOUT){
                       continue;
                   }else{
                       Debug::output("Irrecoverable error. Exiting...");
                       exit(-1);
                   }
               }
          }
         
         
    }
};

#endif //_IMAGEPROCESSOR_H
