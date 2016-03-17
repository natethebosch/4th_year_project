/**
 * Project Realtime Solar Scanner Control
 * @author Nate Bosscher, Tyler d'Entremont
 * @version 1.0
 */


#ifndef _WEBSERVER_H
#define _WEBSERVER_H

#define WEBSERVER_WORKER_POOL_SIZE 10

#include <queue>

#include "../sys/Task.h"
#include "../sys/Mutex.h"
#include "./WebWorker.h"

struct WebTask{
    int socket;
};

class WebWorker;

class WebServer: public Task {
    char* dir;
    WebWorker* workerPool[WEBSERVER_WORKER_POOL_SIZE];
    
    void stopWorkers();
    
public:
    
    static Mutex mu_tasks;
    static std::queue<WebTask*> tasks;
    
    /**
     * Creates webserver at absolute path given by _dir
     */
    WebServer(const char* _dir);
    
    static WebTask* fetchTask(Task *caller);
    static void addTask(WebTask *tsk);
    
    void run(void *args);
};

#endif //_WEBSERVER_H
