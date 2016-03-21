/**
 * Project Realtime Solar Scanner Control
 * @author Nate Bosscher, Tyler d'Entremont
 * @version 1.0
 */


#ifndef _WEBWORKER_H
#define _WEBWORKER_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "../sys/Task.h"
#include "WebServer.h"

#include "./WebWorkerStructs.h"

#define SOCKET_BUFFER_SIZE 512
#define WEB_ROOT "/home/webroot"

class WebWorker : public Task {
    
    std::map<std::string, std::string> mimeTypes;
    std::string _rootDir;
    
    HTTPMethod getMethod(std::string str);
    std::pair<std::string,std::string> getHeaderForString(std::string input);
    
    URL decodeURL(std::string url);
    
    void respond404(int socket);
    void responseInvalid(int socket);
    
    /**
     * get filesize in bytes
     */
    size_t getFileSize(std::string filename){
        std::ifstream file(filename.c_str() , std::ios::binary | std::ios::ate);
        size_t fileSize = file.tellg();
        file.close();
        
        return fileSize;
    }

public:
    WebWorker(char* dir);
    
    static int worker_ct;
    
    /**
     * Takes WebServer* cookie as parameter to listen for incomming socket
     * connections
     */
    void run(void* cookie);
};

#endif //_WEBWORKER_H
