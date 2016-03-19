//
//  AudioRecorder.cpp
//  4th_year_project_r2
//
//  Created by nate bosscher on 2016-03-19.
//  Copyright © 2016 nate bosscher. All rights reserved.
//

#include "AudioRecorder.h"
#include <iostream>

AudioRecorder::AudioRecorder(){
    long loops;
    
    /* Open PCM device for recording (capture). */
    rc = snd_pcm_open(&handle, "default",
                      SND_PCM_STREAM_CAPTURE, 0);
    if (rc < 0) {
        fprintf(stderr,
                "unable to open pcm device: %s\n",
                snd_strerror(rc));
        exit(1);
    }
    
    /* Allocate a hardware parameters object. */
    snd_pcm_hw_params_alloca(&params);
    
    /* Fill it in with default values. */
    snd_pcm_hw_params_any(handle, params);
    
    /* Set the desired hardware parameters. */
    
    /* Interleaved mode */
    snd_pcm_hw_params_set_access(handle, params,
                                 SND_PCM_ACCESS_RW_INTERLEAVED);
    
    /* Signed 8-bit format */
    snd_pcm_hw_params_set_format(handle, params,
                                 SND_PCM_FORMAT_S8);
    
    /* One channel */
    snd_pcm_hw_params_set_channels(handle, params, 1);
    
    /* 44100 bits/second sampling rate (CD quality) */
    val = 44100;
    snd_pcm_hw_params_set_rate_near(handle, params,
                                    &val, &dir);
    
    /* Set period size to 32 frames. */
    frames = 32;
    snd_pcm_hw_params_set_period_size_near(handle,
                                           params, &frames, &dir);
    
    /* Write the parameters to the driver */
    rc = snd_pcm_hw_params(handle, params);
    if (rc < 0) {
        fprintf(stderr,
                "unable to set hw parameters: %s\n",
                snd_strerror(rc));
        exit(1);
    }
    
    /* Use a buffer large enough to hold one period */
    snd_pcm_hw_params_get_period_size(params,
                                      &frames, &dir);
    size = frames * 4; /* 2 bytes/sample, 2 channels */
}

int AudioRecorder::getSampleValue(){
    
    frames = 1;
    char* buffer = (char*)malloc(sizeof(int) * frames);
    
    // get actual period
    snd_pcm_hw_params_get_period_time(params,
                                      &val, &dir);
    
    long loops = 10;
    int count = (int)loops;
    long long outputValue = 0;
    int sample;
    
    while (loops > 0) {
        loops--;
        
        rc = snd_pcm_readi(handle, buffer, frames);
        
        sample = ((int)buffer[0] << 4 | (int)buffer[1]);
        outputValue += sample;
        
        if (rc == -EPIPE) {
            /* EPIPE means overrun */
            fprintf(stderr, "overrun occurred\n");
            snd_pcm_prepare(handle);
        } else if (rc < 0) {
            fprintf(stderr,
                    "error from read: %s\n",
                    snd_strerror(rc));
        } else if (rc != (int)frames) {
            fprintf(stderr, "short read, read %d frames\n", rc);
        }
    }
    
    free(buffer);
    
    return (int)(outputValue / ((float)count));
}

void AudioRecorder::printInfo(){
    int val;
    
    printf("ALSA library version: %s\n",
           SND_LIB_VERSION_STR);
    
    printf("\nPCM stream types:\n");
    for (val = 0; val <= SND_PCM_STREAM_LAST; val++)
        printf("  %s\n",
               snd_pcm_stream_name((snd_pcm_stream_t)val));
    
    printf("\nPCM access types:\n");
    for (val = 0; val <= SND_PCM_ACCESS_LAST; val++)
        printf("  %s\n",
               snd_pcm_access_name((snd_pcm_access_t)val));
    
    printf("\nPCM formats:\n");
    for (val = 0; val <= SND_PCM_FORMAT_LAST; val++)
        if (snd_pcm_format_name((snd_pcm_format_t)val)
            != NULL)
            printf("  %s (%s)\n",
                   snd_pcm_format_name((snd_pcm_format_t)val),
                   snd_pcm_format_description(
                                              (snd_pcm_format_t)val));
    
    printf("\nPCM subformats:\n");
    for (val = 0; val <= SND_PCM_SUBFORMAT_LAST;
         val++)
        printf("  %s (%s)\n",
               snd_pcm_subformat_name((
                                       snd_pcm_subformat_t)val),
               snd_pcm_subformat_description((
                                              snd_pcm_subformat_t)val));
    
    printf("\nPCM states:\n");
    for (val = 0; val <= SND_PCM_STATE_LAST; val++)
        printf("  %s\n",
               snd_pcm_state_name((snd_pcm_state_t)val));
}