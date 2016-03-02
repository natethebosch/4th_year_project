//
//  Testing.cpp
//  4th_year_project_r2
//
//  Created by nate bosscher on 2016-02-27.
//  Copyright © 2016 nate bosscher. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "./TBlockingQueue.h"
#include "./TTask.h"

int main(int argc, char** argv){
    
    int test_number = 0;
    
    std::cout << "Starting test sequence\n\n";
    std::cout.flush();
    
    /**************************
     * Blocking Queue Test
     **************************/
    
    TBlockingQueue* tbq = new TBlockingQueue;
    test_number++;
    
    if(!tbq->test()){
        std::cout << "Test " << test_number << " failed:\n";
        std::cout << tbq->error << "\n";
        std::cout << "\n\n";
    }
    
    delete tbq;
    
    /**************************
     * Task Test
     **************************/
    
    TTask* ttk = new TTask;
    test_number++;
    
    if(!ttk->test()){
        std::cout << "Test " << test_number << " failed:\n";
        std::cout << ttk->error << "\n";
        std::cout << "\n\n";
    }
    
    delete ttk;
    
    std::cout << "Testing Complete!\n";
    
    return 0;
}