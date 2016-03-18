//
//  TWebServer.h
//  4th_year_project_r2
//
//  Created by nate bosscher on 2016-03-17.
//  Copyright © 2016 nate bosscher. All rights reserved.
//

#ifndef TWebServer_h
#define TWebServer_h

#include <stdio.h>
#include <iostream>

#include "../src/tasks/WebServer.h"

#include "./Test.h"

class TWebServer : public Test {
public:
    bool test(){
        
        WebServer* ws = new WebServer("/root/model/test/ws-test");
        ws->start();
        
        printf("Press any key to continue...");
        getchar();
        
        ws->kill();
        
        return true;
    }
};


#endif /* TWebServer_h */
