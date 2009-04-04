// Copyright 2009 Jack Christopher Kastorff

#ifndef _OUTPUT_OPENAL_H_
#define _OUTPUT_OPENAL_H_

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#define OUTPUT_OPENAL_NUM_BUFFERS 10
#define OUTPUT_OPENAL_BUFFER_SIZE 4096

struct output_openal_st {
    float *input;
    ALCshort *buf;
    int bufat;

    ALCdevice *dev;
    ALCcontext *ctx;
    ALuint source;
    ALuint buffers[OUTPUT_OPENAL_NUM_BUFFERS];
    int albuffersused;
    int whichalbuf;
};

void output_openal_make(float *input);

#endif
