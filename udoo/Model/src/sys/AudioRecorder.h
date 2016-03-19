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
    
    snd_pcm_t *handle;
    snd_pcm_uframes_t frames;
    int size;
    int rc;
    snd_pcm_hw_params_t *params;
    unsigned int val;
    int dir;
    
    AudioRecorder();
    
    ~AudioRecorder(){
        snd_pcm_drain(handle);
        snd_pcm_close(handle);
    }
    
    /**
     * returns a 10msec average value
     */
    int getSampleValue();
    
    /**
     * prints ASLA info
     */
    static void printInfo();
};

#endif /* AudioRecorder_hpp */
