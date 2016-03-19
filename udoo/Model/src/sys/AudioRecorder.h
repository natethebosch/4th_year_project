//
//  AudioRecorder.hpp
//  4th_year_project_r2
//
//  Created by nate bosscher on 2016-03-19.
//  Copyright © 2016 nate bosscher. All rights reserved.
//

#ifndef AudioRecorder_hpp
#define AudioRecorder_hpp

#include <stdio.h>

/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API

#include <alsa/asoundlib.h>



class AudioRecorder{
public:
    AudioRecorder(){
        
    }
    
    void record();
    
    /**
     * prints ASLA info
     */
    static void printInfo();
};

#endif /* AudioRecorder_hpp */
