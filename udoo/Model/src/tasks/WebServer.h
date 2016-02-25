/**
 * Project Realtime Solar Scanner Control
 * @author Nate Bosscher, Tyler d'Entremont
 * @version 1.0
 */


#ifndef _WEBSERVER_H
#define _WEBSERVER_H

#include "../sys/Task.h"


class WebServer: public Task {
public: 
    WebWorker workerPool[10];
    
    void run();
};

#endif //_WEBSERVER_H
