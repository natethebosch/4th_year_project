//
//  WebWorkerStructs.h
//  4th_year_project_r2
//
//  Created by nate bosscher on 2016-03-01.
//  Copyright © 2016 nate bosscher. All rights reserved.
//

#ifndef WebWorkerStructs_h
#define WebWorkerStructs_h

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


#endif /* WebWorkerStructs_h */
