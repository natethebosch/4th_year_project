/**
 * Project Realtime Solar Scanner Control
 * @author Nate Bosscher, Tyler d'Entremont
 * @version 1.0
 */


#ifndef _WEBWORKER_H
#define _WEBWORKER_H

#include "../sys/Task.h"
//#include <iostream>
//#include <map>
//#include <string>

#define SOCKET_BUFFER_SIZE 512
#define WEB_ROOT "/home/webroot"

// based on https://www.ietf.org/rfc/rfc2616.txt
enum HTTPMethod {
    OPTIONS,
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    TRACE,
    CONNECT,
    INVALID // for invalid decoding
};

enum HTTPRequestState{
    METHOD,
    PATH,
    HTTP_VERSION,
    HEADER,
    DONE
};

struct URL{
    bool https;
    std::string host;
    std::string path;
    std::string query;
    int port;
};


class WebWorker: public Task {
    std::map<std::string, std::string> mimeTypes;
    
    WebWorker(char* name) : Task(name, 10){
        mimeTypes.insert(std::pair<std::string, std::string>("js", "application/javascript"));
        mimeTypes.insert(std::pair<std::string, std::string>("json", "application/json"));
        mimeTypes.insert(std::pair<std::string, std::string>("html", "text/html"));
        mimeTypes.insert(std::pair<std::string, std::string>("css", "text/css"));
    }
    
    HTTPMethod getMethod(std::string str);
    std::pair<std::string,std::string> getHeaderForString(std::string input);
    
    URL decodeURL(std::string url);
    
    void respond404(int socket);
    void responseInvalid(int socket);
    
public:
    /**
     * Takes WebServer* cookie as parameter to listen for incomming socket
     * connections
     */
    void run(void* cookie);
};

#endif //_WEBWORKER_H
