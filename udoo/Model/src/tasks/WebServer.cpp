/**
 * Project Realtime Solar Scanner Control
 * @author Nate Bosscher, Tyler d'Entremont
 * @version 1.0
 */


#include "WebServer.h"

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <fcntl.h>


/**
 * WebServer implementation
 */
// init static variables
Mutex* WebServer::mu_tasks;
std::queue<WebTask*> WebServer::tasks;

WebServer::WebServer(const char* _dir) : Task("WebServer", 10){
    mu_tasks=new Mutex();
    
    dir = (char*)_dir;
    
    for(int i = 0; i < WEBSERVER_WORKER_POOL_SIZE; i++){
        workerPool[i] = new WebWorker(dir);
    }
}

void WebServer::stopWorkers(){
    std::cout << "Stopping workers...\n";
    
    for(int i = 0; i < WEBSERVER_WORKER_POOL_SIZE; i++){
        workerPool[i]->kill();
    }
    
    std::cout << "Workers stopped...\n";
}

void WebServer::run(void* args) {
    // variables
    int create_socket, new_socket;
    socklen_t addrlen;
    
    WebTask *tsk;
    struct sockaddr_in address;
    
    // create socket
    if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) <= 0){
        Debug::output("Couldn't create socket");
        return;
    }
    
    // set socket as non-blocking
    fcntl(create_socket, F_SETFL, O_NONBLOCK);
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8000);
    
    std::string rebindingMsg;
    
    // loop until we can bind or we reach the end
    int i = 0;
    for(;;){
        // bind socket
        if (bind(create_socket, (struct sockaddr *) &address, sizeof(address)) == 0){
            Debug::output("Binding Socket\n");
            break;
        }else{
            Debug::output("Couldn't Bind Socket\n");
            
            rebindingMsg = "Rebinding (retry ";
            rebindingMsg += std::to_string(i);
            rebindingMsg += ")\n";
            
            Debug::output(rebindingMsg.c_str());
            
            // sleep in the hopes of the socket becomming free
            std::cout << "Sleeping for 1 sec to wait for socket to be free\n";
            sleep_millis(1000);
        }
        
        // check for task signal
        if(hasTerminateSignal()){
            return;
        }
        
        i++;
    }
    
    // start workers
    for(int i = 0; i < WEBSERVER_WORKER_POOL_SIZE; i++){
        workerPool[i]->start();
    }
    
    
    // continuall listen until hasTerminateSignal is true
    while (1) {
        
        // check for task signals
        if(hasTerminateSignal()){
            stopWorkers();
            close(create_socket);
            return;
        }

        while(listen(create_socket, 10) == -1){
            switch(errno){
//                case EAGAIN: -- commented because compiler says EAGAIN == EWOULDBLOCK
                case EWOULDBLOCK:
                    
                    // check for task signals
                    if(hasTerminateSignal()){
                        stopWorkers();
                        close(create_socket);
                        return;
                    }else{
                        yeild();
                    }
                    
                    break;
                case EADDRINUSE:
                    Debug::output("Socket address already in use");
                    return;
                case EBADF:
                    Debug::output("The argument sockfd is not a valid descriptor");
                    return;
                case ENOTSOCK:
                    Debug::output("The argument sockfd is not a socket");
                    return;
                case EOPNOTSUPP:
                    Debug::output("The socket is not of a type that supports the listen() operation");
                    return;
                default:
                    Debug::output("Unknown socket error");
                    return;
            }
        }
        
        addrlen = sizeof(address);
        
        // accept the new connection
        new_socket = accept(create_socket, (struct sockaddr *) &address, &addrlen);
        
        // check for errors
        if (new_socket == -1) {
            switch(errno){
                case EWOULDBLOCK: // , EAGAIN
                    // nothing to do, so yeild to round robin
                    this->yeild();
                    break;
                case EBADF:
                    Debug::output("The descriptor is invalid.");
                    break;
                case ECONNABORTED:
                    Debug::output("A connection has been aborted.");
                    break;
                case EFAULT:
                    Debug::output("The addr argument is not in a writable part of the user address space.");
                    break;
                case EINTR:
                    Debug::output("The system call was interrupted by a signal that was caught before a valid connection arrived; see signal(7).");
                    break;
                case EINVAL:
                    Debug::output("Socket is not listening for connections, or addrlen is invalid (e.g., is negative).");
                    break;
                case EMFILE:
                    Debug::output("The per-process limit of open file descriptors has been reached.");
                    break;
                case ENFILE:
                    Debug::output("The system limit on the total number of open files has been reached.");
                    break;
                case ENOBUFS:
                case ENOMEM:
                    Debug::output("Not enough free memory. This often means that the memory allocation is limited by the socket buffer limits, not by the system memory.");
                    break;
                case ENOTSOCK:
                    Debug::output("The descriptor references a file, not a socket.");
                    break;
                case EOPNOTSUPP:
                    Debug::output("The referenced socket is not of type SOCK_STREAM.");
                    break;
                case EPROTO:
                    Debug::output("Protocol error.");
                    break;
                case EPERM:
                    Debug::output("Firewall rules forbid connection.");
                    break;
                case ENOSR:
                case ESOCKTNOSUPPORT:
                case EPROTONOSUPPORT:
                case ETIMEDOUT:
#ifdef ERESTARTSYS
                case ERESTARTSYS:
#endif
                default:
                    Debug::output("Unknown error");
//                    std::cout << "Unknown error" << errno << "\n";
                    break;
            }
        }
        
        // check if the socket connection worked
        if (new_socket > 0){
            
            // connected
            tsk = (WebTask*)malloc(sizeof(WebTask));
            tsk->socket = new_socket;
            
            addTask(tsk);
        }else{
            
            // No socket bound
            yeild();
            //Debug::output("No socket bound");
        }
    }
}

void WebServer::addTask(WebTask *tsk){
    printf("Trying to lock mu_tasks\n");
    // keep trying until lock is achieved
    while(!WebServer::mu_tasks->lock()){}
    
    printf("Adding task\n");
    tasks.push(tsk);
    
    WebServer::mu_tasks->release();
    printf("Released lock\n");
}

WebTask* WebServer::fetchTask(Task *caller){
    WebTask* tsk = NULL;
    
    // blocks here until a thread is available
    if(WebServer::mu_tasks->lock()){
        if(tasks.empty()){
            WebServer::mu_tasks->release();
            caller->sleep(100);
        }else{
            tsk = tasks.front();
            tasks.pop();
            mu_tasks->release();
            
            return tsk;
        }
    }
    
    return NULL;
}
