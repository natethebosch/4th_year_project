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

#include "tasks/ImageProcessor.h"
#include "tasks/MotionGuard.h"
#include "tasks/MotorControl.h"
#include "tasks/WebServer.h"
#include "tasks/SensorCommunicator.h"
#include "./helpers/BlockingQueue.h"

/**
 * Main implementation
 */


void Main::initial() {
    BlockingQueue<SensorDataPoint> queue("Serial2ImageProcessorQueue");
    
    SensorCommunicator comm(queue.getSender());
    ImageProcessor imgProcessor(queue.getReceiver());
    
    // more to go here
    
    
    // start tasks
    comm.startPeriodic(SENSOR_SERIAL_PERIODIC_WATCH);
    imgProcessor.start();
}