//
//  TAudio.hpp
//  4th_year_project_r2
//
//  Created by nate bosscher on 2016-03-19.
//  Copyright © 2016 nate bosscher. All rights reserved.
//

#ifndef TAudio_hpp
#define TAudio_hpp

#include <stdio.h>
#include <iostream>
#include "../src/sys/AudioRecorder.h"

class TAudio : public Test{
    
public:
    bool test(){
        
        AudioRecorder ar;
        
        AudioRecorder::printInfo();
        std::cout << "Starting record...\n";
        ar.record();
        
    }
};

#endif /* TAudio_hpp */
