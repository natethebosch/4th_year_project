/**
 * Project Realtime Solar Scanner Control
 * @author Nate Bosscher, Tyler d'Entremont
 * @version 1.0
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include "Main.h"

#include "./tasks/ImageProcessor.h"
#include "./tasks/MotionGuard.h"
#include "./tasks/MotorControl.h"
#include "./tasks/WebServer.h"
#include "./tasks/SensorCommunicator.h"
#include "./helpers/BlockingQueue.h"
#include "./tasks/MotorControl.h"
#include "./type/SensorDataPoint.h"



/**
 * Main implementation
 */


void Main::initial() {
    BlockingQueue<SensorDataPoint>* queue;
    queue = new BlockingQueue<SensorDataPoint>("Serial2ImageProcessorQueue");
    
    SensorCommunicator comm(queue);
    ImageProcessor imgProcessor(queue);
    WebServer ws("/home/webdir");
    
    // more to go here
    
    
    // start tasks
    comm.startPeriodic(SENSOR_SERIAL_PERIODIC_WATCH);
    imgProcessor.start();
}

int main(int argc, char** argv){
    Main m;
    m.initial();
    
    return 0;
}
