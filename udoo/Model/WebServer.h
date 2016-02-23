/**
 * Project Realtime Solar Scanner Control
 * @author Nate Bosscher, Tyler d'Entremont
 * @version 1.0
 */


#ifndef _WEBSERVER_H
#define _WEBSERVER_H

#include "Task.h"


class WebServer: public Task {
public: 
    void workerPool;
    
    void run();
};

#endif //_WEBSERVER_H