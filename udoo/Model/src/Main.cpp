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
        

    Debug::output("Starting main...");
    BlockingQueue<SensorDataPoint> *queue;
    queue = new BlockingQueue<SensorDataPoint>("ImageProcessorQueue");
    Debug::output("Created blocking-queue");

    ImageProcessor imgProcessor(queue, std::string(WEB_ROOT));
    Debug::output("Created ImageProcessor");
    WebServer ws(WEB_ROOT);
    Debug::output("Created Webserver");
    
    Debug::output("Started ImageProcessor");
    imgProcessor.start();

    Debug::output("Creating ctrl");
    
    Control ctrl(queue);
    Debug::output("Ctrl created");
    ctrl.start();
    Debug::output("started ctrl");
    
    Debug::output("Waiting for join");
    ctrl.join();
//    imgProcessor.join();
    ws.join();
    Debug::output("Done!!");
}

int main(int argc, char** argv){
    Main m;
    m.initial();
    
    return 0;
}
