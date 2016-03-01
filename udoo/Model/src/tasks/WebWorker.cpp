/**
 * Project Realtime Solar Scanner Control
 * @author Nate Bosscher, Tyler d'Entremont
 * @version 1.0
 */

#include "WebWorker.h"

//#include <stdio.h>
//#include <stdlib.h>
//#include <sys/stat.h>
//#include <sys/types.h>

/**
 * WebWorker implementation
 */

// init static variables
Mutex WebServer::mu_tasks;
std::queue<WebTask*> WebServer::tasks;

WebWorker::WebWorker(char* name) : Task(name, 10){
    mimeTypes.insert(std::pair<std::string, std::string>("js", "application/javascript"));
    mimeTypes.insert(std::pair<std::string, std::string>("json", "application/json"));
    mimeTypes.insert(std::pair<std::string, std::string>("html", "text/html"));
    mimeTypes.insert(std::pair<std::string, std::string>("css", "text/css"));
}

HTTPMethod WebWorker::getMethod(std::string str){
    if(str == std::string("OPTIONS"))
        return OPTIONS;
    if(str == std::string("GET"))
        return GET;
    if(str == std::string("HEAD"))
        return HEAD;
    if(str == std::string("POST"))
        return POST;
    if(str == std::string("PUT"))
        return PUT;
    if(str == std::string("DELETE"))
        return DELETE;
    if(str == std::string("TRACE"))
        return TRACE;
    if(str == std::string("CONNECT"))
        return CONNECT;
    return INVALID;
}

std::pair<std::string, std::string> WebWorker::getHeaderForString(std::string input){
    
    size_t p = input.find_first_of(':');
    std::string name = input.substr(0, p);
    std::string value = input.substr(p+1, std::string::npos);
    
    return std::pair<std::string, std::string>(name, value);
}

// todo resolve hex values in url
URL WebWorker::decodeURL(std::string url){
    URL u;
    size_t p0, p1;
    
    p0 = 0;
    p1 = 0;
    
    u.query = "";
    u.https = false;
    u.port = -1;
    
    // check for wildcard
    if(url == std::string("*")){
        u.host = "";
        u.path = "/";
        return u;
    }
    
    // check if it's an absolute path
    bool isAbs = false;
    bool isHttps = false;
    if(url.find_first_of("http://") != std::string::npos){
        isAbs = true;
    }else if(url.find_first_of("https://") != std::string::npos){
        isAbs = true;
        isHttps = true;
        u.https = true;
    }
    
    // absolute path
    if(isAbs){
        if(isHttps)
            p0 = url.find_first_of("https://");
        else
            p0 = url.find_first_of("http://");
        
        // check for port ie. http://www.google.ca:80/asdfa
        p1 = url.find_first_of(':', p0);
        if(p1 != std::string::npos){
            u.host = url.substr(p0, p1-p0);
            
            // convert to lower case
            std::transform(u.host.begin(), u.host.end(), u.host.begin(), ::tolower);
            p0 = p1 + 1;
            
            // decode port
            p1 = url.find_first_of('/', p0);
            if(p1 == std::string::npos)
                u.port = atoi(url.substr(p0, p1).c_str());
            else
                u.port = atoi(url.substr(p0, p1 - p0).c_str());
            
            if(p1 == std::string::npos){
                u.path = "/";
                return u;
            }
            
            p0 = p1;
        }else{
            // no port, just check for end of hostname
            p1 = url.find_first_of("/", p0);
            if(p1 == std::string::npos)
                u.host = url.substr(p0, p1);
            else
                u.host = url.substr(p0, p1-p0);
            
            // convert to lower case
            std::transform(u.host.begin(), u.host.end(), u.host.begin(), ::tolower);
            
            // no ending slash for hostname
            if(p1 == std::string::npos){
                u.path = "/";
                return u;
            }
            
            p0 = p1;
        }
    }
    
    // continue with path
    
    p0 = p1;
    
    p1 = url.find_first_of('?');
    if(p1 == std::string::npos){
        u.path = url.substr(p0, p1);
        return u;
    }else{
        u.path = url.substr(p0, p1 - p0);
        u.query = url.substr(p1+1, std::string::npos);
    }
    
    return u;
}

void WebWorker::respond404(int socket){
    std::string buff = "404 Not Found\r\n";
    write(socket, buff.c_str(), buff.size());
}

void WebWorker::responseInvalid(int socket){
    std::string buff = "500 Internal Server Error\r\n";
    write(socket, buff.c_str(), buff.size());
}

void WebWorker::run(void *cookie){
    WebServer* ws = (WebServer*)cookie;
    
    char* buffer = (char*)malloc(sizeof(char) * SOCKET_BUFFER_SIZE);

    std::string pad = "";
    std::string buff;
    std::string sUrl;
    std::string ext;
    
    std::map<std::string, std::string>::iterator it;
    
    WebTask* wt;
    
    HTTPMethod method;
    HTTPRequestState state;
    URL url;
    
    std::map<std::string,std::string> headers;
    
    bool continueDecoding;
    bool invalidEncoding;
    size_t p, p2;
    
    for(;;){
        wt = ws->fetchTask(this);
        
        // check for no task (some internal error happened)
        if(wt == NULL)
            continue;

        
        recv(wt->socket, buffer, SOCKET_BUFFER_SIZE, 0);
        buff = buffer;
        
        // decode request
        p = 0;
        state = METHOD;
        method = INVALID;
        continueDecoding = true;
        invalidEncoding = false;
        
        while(buff[p] != '\0' && !invalidEncoding && continueDecoding){
            switch(state){
                case METHOD:
                    p2 = buff.find_first_of(" ");
                    if(p2 == std::string::npos){
                        invalidEncoding = true;
                        break;
                    }
                    
                    method = getMethod(buff.substr(p, p2 - p));
                    if(method != GET){
                        continueDecoding = false;
                    }else{
                        p = p2;
                    }
                    state = PATH;
                    break;
                case PATH:
                    p2 = buff.find_first_of(" ", p);
                    if(p2 == std::string::npos){
                        invalidEncoding = true;
                        break;
                    }
                    
                    sUrl = buff.substr(p, p2 - p);
                    p = p2;
                    state = HTTP_VERSION;
                    break;
                case HTTP_VERSION:
                    p2 = buff.find_first_of("\r\n", p);
                    if(p2 == std::string::npos){
                        invalidEncoding = true;
                        break;
                    }
                    p = p2;
                    break;
                case HEADER:
                    p2 = buff.find_first_of("\r\n", p);
                    
                    if(p2 == std::string::npos){
                        invalidEncoding = true;
                        break;
                    }
                    if(p2 == p){
                        state = DONE;
                        break;
                    }
                    
                    headers.insert(getHeaderForString(buff.substr(p, p2 - p)));
                    
                    p = p2;
                    
                    break;
                case DONE:
                    break;
            }
            
            p++;
        }
        
        // check if there was an error while decoding
        if(!invalidEncoding){
            responseInvalid(wt->socket);
            close(wt->socket);
            free(wt);
            continue;
        }
        
        if(method != GET){
            respond404(wt->socket);
            close(wt->socket);
            free(wt);
            continue;
        }
        
        url = decodeURL(sUrl);
        
        // todo check for host name in either absolute url or header
        if(url.path == std::string("/"))
            url.path = "/index.html";
        else
            url.path = WEB_ROOT + url.path;
        
        std::ifstream myfile;
        myfile.open (url.path.c_str(), std::ios::in);
        
        if(!myfile.is_open()){
            respond404(wt->socket);
            
            myfile.close();
            close(wt->socket);
            free(wt);
            continue;
        }
        
        buff = "200 OK\r\n";
        write(wt->socket, buff.c_str(), buff.size());
        
        p = url.path.find_last_of('.');
        if(p != std::string::npos){
            ext = url.path.substr(p, std::string::npos);
            
            buff = "Content-Type: ";
            buff += mimeTypes.find(ext)->second;
            buff += "\r\n";
            
            write(wt->socket, buff.c_str(), buff.size());
        }
        
        buff = "\r\n";
        write(wt->socket, buff.c_str(), buff.size());
        
        while(getline(myfile, buff)){
            write(wt->socket, buff.c_str(), buff.size());
        }
        
        
        // garbage cleanup
        myfile.close();
        close(wt->socket);
    
        free(wt);
    }
}