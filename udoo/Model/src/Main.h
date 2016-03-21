/**
 * Project Realtime Solar Scanner Control
 * @author Nate Bosscher, Tyler d'Entremont
 * @version 1.0
 */


#ifndef _MAIN_H
#define _MAIN_H

#include "constants.h"
#include "Magick++.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include <signal.h>
#include <iostream>
#include <sys/mman.h>

// xenomai
#include <task.h>
#include <timer.h>

#include "./tasks/ImageProcessor.h"
#include "./tasks/MotionGuard.h"
#include "./tasks/Control.h"
#include "./tasks/WebServer.h"
#include "./helpers/BlockingQueue.h"
#include "./type/SensorDataPoint.h"

#define WEB_ROOT "/home/webroot"

/**
 * Main implementation
 */

// xenomai task
RT_TASK task;

void run_main_prog(void *args) {
    
    
    Debug::output("Starting main...");
    BlockingQueue<SensorDataPoint> queue("ImageProcessorQueue", 1000);
    BlockingQueue<SensorDataPoint> *_queue = &queue;
    Debug::output("Created blocking-queue");
    
    printf("BQ = %p\n", queue);
    std::cout.flush();

    std::string web_root = std::string(WEB_ROOT);
    ImageProcessor *imgProcessor;
    imgProcessor = new ImageProcessor(_queue, web_root);
    Debug::output("Created ImageProcessor");

    WebServer *ws;
    ws = new WebServer(WEB_ROOT);
    Debug::output("Created Webserver");
    
    Debug::output("Started ImageProcessor");
    imgProcessor->start();

    Debug::output("Creating ctrl");
    
    Control ctrl(_queue);
    Debug::output("Ctrl created");
    ctrl.start();
    Debug::output("started ctrl");
    
    Debug::output("Waiting for join");
    ctrl.join();
    imgProcessor->join();
    ws->join();
    Debug::output("Done!!");
}

int main(int argc, char** argv){
    
    Magick::InitializeMagick(*argv);
    
    // lock memory
    mlockall(MCL_CURRENT|MCL_FUTURE);
    
    rt_task_create(&task, "main", 0, 99, T_JOINABLE);
    rt_task_start(&task, &run_main_prog, NULL);
    
    // wait for task to complete
    rt_task_join(&task);
    rt_task_delete(&task);
    
    std::cout.flush();
    
    return 0;
}

#endif //_MAIN_H
