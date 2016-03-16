
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include "../src/tasks/ImageProcessor.h"


void testImg(){
  	SensorDataPoint dp;
    BlockingQueue<SensorDataPoint>* queue;
    queue = new BlockingQueue<SensorDataPoint>("Serial2ImageProcessorQueue");
    
    ImageProcessor imgProcessor(queue);
    imgProcessor.start();
    

    
    for (int i=0; i<24; i++){
    	for (int n=0;n<24; n++){
    		dp.y=i;
    		dp.x=n;
    		dp.value=i*n;
    		
    		queue->put (&dp);
		}
	}
}

