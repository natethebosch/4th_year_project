
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include "./Test.h"
#include "../src/tasks/ImageProcessor.h"

class ImageTest: public Test {
public:	
virtual bool test(int i);
};

bool ImageTest::test(int i)
{
	SensorDataPoint* dp;
    BlockingQueue<SensorDataPoint>* queue;
    queue = new BlockingQueue<SensorDataPoint>("Serial2ImageProcessorQueue");
    ImageProcessor imgProcessor(queue);
    imgProcessor.start();

    dp=(SensorDataPoint*)malloc(sizeof(SensorDataPoint));
    for (int i=0; i<24; i++){
    	for (int n=0;n<24; n++){
    		
    		dp->y=i*20;
    		dp->x=n*20;
    		dp->value=(i+n)*20;
    		
    		queue->put(dp);
		}
	}
	free(dp);
	return true;
}
