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
    float *trigger;
};

// !lua:input_sndfile -> input_sndfile_make(float *trigger, char *path, bool startnow)
float * input_sndfile_make(float *trigger, char *path, int startnow);

#endif

