//
//  TTask.h
//  4th_year_project_r2
//
//  Created by nate bosscher on 2016-02-27.
//  Copyright © 2016 nate bosscher. All rights reserved.
//

#ifndef TTask_h
#define TTask_h

#include "./Test.h"
#include "../src/sys/Task.h"

class TTaskJob : public Task{
public:
    // cookie should be an *int
    void run(void* cookie){
        
    }
};

class TTask : public Test{
public:
    bool test(){
        
        Task t* = new Task("Some Task", 10);
        
        t->start();
        
        Task t3* = new Task("Some Other Task", 80);
        
        
    }
};

#endif /* TTask_h */
