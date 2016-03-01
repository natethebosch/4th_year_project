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

/**
 * WebServer implementation
 */


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
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(80);
    
    if (bind(create_socket, (struct sockaddr *) &address, sizeof(address)) == 0){
        printf("Binding Socket\n");
    }
    
    
    while (1) {
        if (listen(create_socket, 10) == -1) {
            switch(errno){
                case EADDRINUSE:
                    Debug::output("Socket address already in use");
                    break;
                case EBADF:
                    Debug::output("The argument sockfd is not a valid descriptor");
                    break;
                case ENOTSOCK:
                    Debug::output("The argument sockfd is not a socket");
                    break;
                case EOPNOTSUPP:
                    Debug::output("The socket is not of a type that supports the listen() operation");
                    break;
                default:
                    Debug::output("Unknown socket error");
                    break;
            }
            return;
        }
        
        
        
        if ((new_socket = accept(create_socket, (struct sockaddr *) &address, &addrlen)) == -1) {
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
                    break;
            }
        }
        
        if (new_socket > 0){
            // connected
            tsk = (WebTask*)malloc(sizeof(WebTask));
            tsk->socket = new_socket;
            
            addTask(tsk);
        }
    }
    
    close(create_socket);    

}

void WebServer::addTask(WebTask *tsk){
    // keep trying until lock is achieved
    while(!WebServer::mu_tasks.lock()){}
    
    tasks.push(tsk);
    
    WebServer::mu_tasks.release();
}

WebTask* WebServer::fetchTask(Task *caller){
    WebTask* tsk = NULL;
    
    // blocks here until a thread is available
    if(WebServer::mu_tasks.lock()){
        if(tasks.empty()){
            WebServer::mu_tasks.release();
            caller->sleep(100);
        }else{
            tsk = tasks.front();
            tasks.pop();
            mu_tasks.release();
            
            return tsk;
        }
    }
    
    return NULL;
}