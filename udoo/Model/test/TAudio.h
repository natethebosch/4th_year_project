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
        std::cout << "Starting record...\n";
        
        for(size_t i = 0; i < 1000; i++){
            std::cout << ar.getSampleValue() << "\n";
        }
        
        std::cout.flush();
        
        return true;
        
    }
};

#endif /* TAudio_hpp */
