//
//  TWebServer.h
//  4th_year_project_r2
//
//  Created by nate bosscher on 2016-03-17.
//  Copyright © 2016 nate bosscher. All rights reserved.
//

#ifndef TWebServer_h
#define TWebServer_h

#include <stdio.h>
#include <iostream>

#include "../src/tasks/WebServer.h"

#include "./Test.h"

class TWebServer : public Test {
public:
    bool test(){
        BlockingQueue<SensorDataPoint> _queue("TEST_QUEUE", 1000);
        BlockingQueue<SensorDataPoint> *queue = &_queue;
        
        ImageProcessor *ip = new ImageProcessor(queue, "/root/ws-test");
        
        WebServer* ws = new WebServer("/home/ws-test");
        ws->start();
        ip->start();
        
        SensorDataPoint sdp;
        
        for(int x = 0; x < 23; x++){
            for(int y = 0; y < 50; y++){
                sdp.x = x;
                sdp.y = y;
                sdp.value = ((float)y) * 1024.0/50.0;
                queue->put(sdp);
            }
        }
        

        ws->join();
        
        return true;
    }
};


#endif /* TWebServer_h */
