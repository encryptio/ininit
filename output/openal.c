/*
 * Copyright (c) 2009 Jack Christopher Kastorff
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions, and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * The name Chris Kastorff may not be used to endorse or promote
 *       products derived from this software without specific prior written
 *       permission.
 */

#include <stdlib.h>
#include <unistd.h>

#include "output/openal.h"
#include "die.h"
#include "helpers.h"

void output_openal_ticker(void * info) {
    struct output_openal_st *me = (struct output_openal_st *)info;
    ALint val;
    int lv, rv;

    lv = 32768 * *me->left;
    rv = 32768 * *me->right;
    if ( lv > 32767 ) lv = 32767;
    if ( rv > 32767 ) rv = 32767;
    if ( lv < -32767 ) lv = -32767;
    if ( rv < -32767 ) rv = -32767;
    // WARNING: this assumes that OUTPUT_OPENAL_BUFFER_SIZE is a multiple of 2*sizeof(ALCshort) = 4.
    // If this assumption is broken, data may be written into unallocated space.
    me->buf[me->bufat++] = (ALCshort) lv;
    me->buf[me->bufat++] = (ALCshort) rv;

    if ( me->bufat >= OUTPUT_OPENAL_BUFFER_SIZE/sizeof(ALCshort) ) {
        if ( me->albuffersused < OUTPUT_OPENAL_NUM_BUFFERS ) {
            alBufferData(me->buffers[me->whichalbuf], AL_FORMAT_STEREO16, me->buf, OUTPUT_OPENAL_BUFFER_SIZE, (ALuint) *sample_rate);
            alSourceQueueBuffers(me->source, 1, &me->buffers[me->whichalbuf]);
            me->whichalbuf++;
            me->albuffersused++;
        } else {
            alGetSourcei(me->source, AL_SOURCE_STATE, &val);
            if ( val != AL_PLAYING ) {
                alSourcePlay(me->source);
                if ( alGetError() == AL_INVALID_OPERATION )
                    die("ctx");
            }

            while ( 1 ) {
                alGetSourcei(me->source, AL_BUFFERS_PROCESSED, &val);
                if ( val > 0 ) break;
                usleep(OUTPUT_OPENAL_WAIT_TIME);
            }

            alSourceUnqueueBuffers(me->source, 1, me->buffers);
            alBufferData(me->buffers[me->whichalbuf], AL_FORMAT_STEREO16, me->buf, OUTPUT_OPENAL_BUFFER_SIZE, (ALuint) *sample_rate);
            alSourceQueueBuffers(me->source, 1, &me->buffers[me->whichalbuf]);
            me->whichalbuf++;

            alGetSourcei(me->source, AL_SOURCE_STATE, &val);
            if ( val != AL_PLAYING ) {
                alSourcePlay(me->source);
                if ( alGetError() == AL_INVALID_OPERATION )
                    die("ctx");
            }
        }

        me->bufat = 0;

        if ( me->whichalbuf >= OUTPUT_OPENAL_NUM_BUFFERS )
            me->whichalbuf = 0;
    }
}

void output_openal_death(void * info) {
    struct output_openal_st *me = (struct output_openal_st *)info;
    ALint val;

    // wait for the buffers to play out
    do {
        alGetSourcei(me->source, AL_SOURCE_STATE, &val);
        usleep(5000);
    } while ( val == AL_PLAYING );

    // ew, not cleaning up after ourselves
}

void output_openal_make(float *left, float *right) {
    struct output_openal_st *ret;

    if ( (ret = malloc(sizeof(*ret))) == NULL )
        die("output_openal_make: couldn't malloc ret");

    if ( (ret->dev = alcOpenDevice(NULL)) == NULL )
        die("output_openal_make: alcOpenDevice failed");

    if ( (ret->ctx = alcCreateContext(ret->dev,NULL)) == NULL )
        die("output_openal_make: alcCreateContext failed");

    alcMakeContextCurrent(ret->ctx);

    alGenBuffers(OUTPUT_OPENAL_NUM_BUFFERS, ret->buffers);
    alGenSources(1, &ret->source);
    if ( alGetError() != AL_NO_ERROR )
        die("output_openal_make: couldn't generate things");
    ret->albuffersused = 0;
    ret->left = left;
    ret->right = right;
    ret->whichalbuf = 0;

    if ( (ret->buf = malloc(OUTPUT_OPENAL_BUFFER_SIZE)) == NULL )
        die("output_openal_make: couldn't malloc space for ret->buf");
    ret->bufat = 0;

    // and set up the sampler
    ii_sampler_call(output_openal_ticker, (void *)ret);
    ii_death_call(output_openal_death, (void *)ret);
}

