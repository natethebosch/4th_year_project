/**
 * Project Realtime Solar Scanner Control
 * @author Nate Bosscher, Tyler d'Entremont
 * @version 1.0
 */


#ifndef _WEBSERVER_H
#define _WEBSERVER_H

#include <queue>

#include "../sys/Task.h"
#include "../sys/Mutex.h"
#include "./WebWorker.h"

struct WebTask{
    int socket;
};

class WebWorker;

class WebServer: public Task {
    WebWorker* workerPool[10];
    
public:
    
    static Mutex mu_tasks;
    static std::queue<WebTask*> tasks;
    
    WebServer() : Task("WebServer", 10){    }
    
    static WebTask* fetchTask(Task *caller);
    static void addTask(WebTask *tsk);
    
    void run(void *args);
};

#endif //_WEBSERVER_H
