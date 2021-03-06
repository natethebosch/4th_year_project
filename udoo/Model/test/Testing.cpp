//
//  Testing.cpp
//  4th_year_project_r2
//
//  Created by nate bosscher on 2016-02-27.
//  Copyright © 2016 nate bosscher. All rights reserved.
//

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <iostream>
#include <sys/mman.h>

// xenomai
#include <task.h>
#include <timer.h>

#include "./TBlockingQueue.h"
#include "./TTask.h"
#include "./TImage.h"
#include "./TWebServer.h"
#include "./TAudio.h"


#include <Magick++.h>

using namespace std;

// xenomai task
RT_TASK task;

void test(void *args){
    int test_number = 0;
//    
//    std::cout << "Starting test sequence\n\n";
//    std::cout.flush();
//
//    /**************************
//     * Blocking Queue Test
//     **************************/
//    
//    TBlockingQueue* tbq = new TBlockingQueue;
//    test_number++;
//    
//    if(!tbq->test()){
//        std::cout << "Test " << test_number << " failed:\n";
//        std::cout << tbq->error << "\n";
//        std::cout << "\n\n";
//    }else{
//    	std::cout << "Test " << test_number << " succeeded!\n\n";
//	}
//    
//    delete tbq;
//    
//    /**************************
//     * Task Test
//     **************************/
//    
//    TTask* ttk = new TTask;
//    test_number++;
//    
//    if(!ttk->test()){
//        std::cout << "Test " << test_number << " failed:\n";
//        std::cout << ttk->error << "\n";
//        std::cout << "\n\n";
//    }else{
//    	std::cout << "Test " << test_number << " succeeded!\n\n";
//	}
//    
//    delete ttk;
//
//    
//    /**************************
//     * Image Processor Test
//     **************************/
//    
//    std::cout <<"Testing ImageProcessor\n";
//    
//    ImageTest* imgtest= new ImageTest;
//    test_number++;
//    
//    if(!imgtest->test()){
//        std::cout << "Test " << test_number << " failed:\n";
//        std::cout << imgtest->error << "\n";
//        std::cout << "\n\n";
//
//    }
//    
//    /**************************
//     * Web server Test
//     **************************/
    
    TWebServer* twb = new TWebServer();
    test_number++;
    
    if(!twb->test()){
        std::cout << "Test " << test_number << " failed:\n";
        std::cout << twb->error << "\n";
        std::cout << "\n\n";
    }
    
    /**************************
     * Audio Test
     **************************/

    printf("Hello world!");
    TAudio *ta = new TAudio();
    test_number++;
    
    if(!ta->test()){
        std::cout << "Test " << test_number << " failed:\n";
        std::cout << ta->error << "\n";
        std::cout << "\n\n";
    }
    
    std::cout << "Testing Complete!\n";
    
}

int main(int argc, char** argv){
    
    
    Magick::InitializeMagick(*argv);
    
    printf("Hello world1\n");
    
    /* Avoids memory swapping for this program */
    mlockall(MCL_CURRENT|MCL_FUTURE);
    
    printf("Hello world2\n");
    
    rt_task_create(&task, "test", 0, 99, T_JOINABLE);
    rt_task_start(&task, &test, NULL);
    
    // wait for task to complete
    rt_task_join(&task);
    rt_task_delete(&task);
    
    std::cout.flush();
    
    return 0;
}
