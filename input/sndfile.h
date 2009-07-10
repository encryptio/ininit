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

