
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include "../src/tasks/ImageProcessor.h"

class ImageTest: public Test {
public:	
virtual bool test()
{
	Debug::output("acess imagetest\n");
	SensorDataPoint dp;
	Debug::output("made sensor data point\n");
    BlockingQueue<SensorDataPoint>* queue;
    Debug::output("initialised queue\n");
    queue = new BlockingQueue<SensorDataPoint>("Serial2ImageProcessorQueue");
    Debug::output("made queue\n");
    ImageProcessor imgProcessor(queue);
    Debug::output("made image processor\n");
    imgProcessor.start();
    Debug::output("started image processor\n");

    
    for (int i=0; i<24; i++){
    	for (int n=0;n<24; n++){
    		dp.y=i;
    		dp.x=n;
    		dp.value=(i+n)*20;
    		
    		queue->put (&dp);
		}
	}
	return true;
}
};
