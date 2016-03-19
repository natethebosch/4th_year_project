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
#include "./tasks/Control.h"
#include "./tasks/WebServer.h"
#include "./helpers/BlockingQueue.h"
#include "./type/SensorDataPoint.h"



/**
 * Main implementation
 */


void Main::initial() {
    BlockingQueue<SensorDataPoint> *queue;
    queue = new BlockingQueue<SensorDataPoint>("ImageProcessorQueue");
    
//    ImageProcessor imgProcessor(queue, std::string(WEB_ROOT));
    WebServer ws(WEB_ROOT);
    
//    imgProcessor.start();
    
    Control ctrl(queue);
    ctrl.start();
    
    ctrl.join();
//    imgProcessor.join();
    ws.join();
}

int main(int argc, char** argv){
    Main m;
    m.initial();
    
    return 0;
}
