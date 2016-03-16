//
//  TTask.h
//  4th_year_project_r2
//
//  Created by nate bosscher on 2016-02-27.
//  Copyright © 2016 nate bosscher. All rights reserved.
//

#ifndef TTask_h
#define TTask_h

// xenomai
#include <timer.h>

// usleep
#include <unistd.h>
// sqrt
#include <math.h>

#include "./Test.h"
#include "../src/sys/Task.h"

class TTaskJob : public Task{
public:
    float result;
    RTIME completedAt = 0;
    
    TTaskJob(const char* taskName, int priority) : Task(taskName, priority){
        // nothing here
    }
    
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
    
    TTaskJob2(const char* taskName, int priority) : Task(taskName, priority){
        // nothing here
    }
    
    // cookie should be an *int
    void run(void* cookie){
        result = *((int*)cookie);
		std::cout << "Got cookie value '" << result << "'\n";
		std::cout.flush();
    }
};



class TTask : public Test{
public:
    bool test(){
        
        std::cout << "Phase 1 - Scheduling\n";
        
        TTaskJob *ta0 = new TTaskJob("Some Task", 10);
        ta0->start(nullptr);
        
        TTaskJob *ta1 = new TTaskJob("Some Other Task", 80);
        
        ta1->start(nullptr);
        
        std::cout << "Sleeping for 2 seconds while tasks complete\n";
        std::cout.flush();
        usleep(2000);
        
        printf("Current time: %l \t ta0 completed at: %l \t ta1 completed at: %l\n", rt_timer_read(), ta0->completedAt, ta1->completedAt);
        
        // t0 should have completed after t1
        if(ta0->completedAt < ta1->completedAt){
            return false;
        }
        
        std::cout << "Phase 2 - Cookie passing\n";
        
        TTaskJob2 *tb0 = new TTaskJob2("Some bTask", 10);
        int *value = (int*) malloc(sizeof(int));
		*value = rand();
        
        std::cout << "passing cookie value of '" << *value << "'\n";
        
        tb0->start(value);
        usleep(10);

        // should be the same value
        if(tb0->result != *value){
        	std::cout << "got result of " << tb0->result << " expeced " << value << "\n";
        	std::cout.flush();
            return false;
        }
        
        return true;
    }
};

#endif /* TTask_h */
