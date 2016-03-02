//
//  TTask.h
//  4th_year_project_r2
//
//  Created by nate bosscher on 2016-02-27.
//  Copyright © 2016 nate bosscher. All rights reserved.
//

#ifndef TTask_h
#define TTask_h

#include <native/timer.h>
#include "./Test.h"
#include "../src/sys/Task.h"

class TTaskJob : public Task{
public:
    float result;
    RTIME completedAt = 0;
    
    // cookie should be an *int
    void run(void* cookie){
        result = 0;
        float sqrt12 = sqrt(12);
        
        for(size_t i = 0; i < 1e6; i++){
            result += sqrt12 * pow(-3.0, i) / (2.0 * (float)i + 1);
        }
        
        completedAt = rt_timer_read();
    }
};

class TTaskJob2 : public Task{
public:
    int result;
    
    // cookie should be an *int
    void run(void* cookie){
        result = *(int*)cookie;
    }
};



class TTask : public Test{
public:
    bool test(){
        
        std::cout << "Phase 1 - Scheduling\n";
        
        TTaskJob ta0* = new TTaskJob("Some Task", 10);
        
        ta0->start(1);
        
        TTaskJob ta1* = new TTaskJob("Some Other Task", 80);
        
        ta1->start(2);
        
        std::cout << "Sleeping for 2 seconds while tasks complete\n";
        std::cout.flush();
        sleep(2);
        
        // t0 should have completed after t1
        if(ta0->completedAt < ta1->completedAt){
            return false;
        }
        
        std::cout << "Phase 2 - Cookie passing\n";
        
        TTaskJob2 tb0* = new TTaskJob2("Some bTask", 10);
        int value = rand();
        
        tb0->start(&value);
        usleep(1);

        // should be the same value
        if(tb0->result != value){
            return false;
        }
        
        return true;
    }
};

#endif /* TTask_h */
