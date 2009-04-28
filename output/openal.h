// Copyright 2009 Jack Christopher Kastorff

#ifndef _OUTPUT_OPENAL_H_
#define _OUTPUT_OPENAL_H_

#ifdef OpenAL_DARWIN
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif

// are there other forms of OpenAL headers?
#ifdef OpenAL_NORMAL
#include <AL/al.h>
#include <AL/alc.h>
#endif

#define OUTPUT_OPENAL_NUM_BUFFERS 10
// WARNING: OUTPUT_OPENAL_BUFFER_SIZE should always be a multiple of 2*sizeof(ALCshort) = 4
// for reasons explained in output/openal.c
#define OUTPUT_OPENAL_BUFFER_SIZE 4096

struct output_openal_st {
    float *left;
    float *right;
    ALCshort *buf;
    int bufat;

    ALCdevice *dev;
    ALCcontext *ctx;
    ALuint source;
    ALuint buffers[OUTPUT_OPENAL_NUM_BUFFERS];
    int albuffersused;
    int whichalbuf;
};

void output_openal_make(float *left, float *right);

#endif
