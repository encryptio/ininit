// Copyright 2009 Jack Christopher Kastorff

#ifndef _input_sndfile_H_
#define _input_sndfile_H_

#include <sndfile.h>

#define INPUT_SNDFILE_BUFFERSIZE 1024

struct input_sndfile_st {
    float now;
    float buffer[INPUT_SNDFILE_BUFFERSIZE];
    int bufferused, bufferat;
    SNDFILE *snd;
    char lastdown;
    int channelcount;
    int channel;
    float *trigger;
};

// !lua:input_sndfile -> input_sndfile_make(float *trigger, char *path, int channel, bool startnow)
/* !doc:input_sndfile(trigger, path, channel, startnow)
 *      Opens the audio file at path and reads the given channel. If startnow
 *      is true, it plays instantly. The trigger tells when to start the sound
 *      over - when its absolute value is greater than 0.5.
 */
float * input_sndfile_make(float *trigger, char *path, int channel, int startnow);

#endif

