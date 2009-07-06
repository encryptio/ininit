// Copyright 2009 Jack Christopher Kastorff

#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "input/sndfile.h"
#include "helpers.h"
#include "die.h"

void input_sndfile_ticker(void * info) {
    struct input_sndfile_st *me = (struct input_sndfile_st *)info;
    int ct;
    
    if ( fabsf(*me->trigger) > 0.5 ) {
        if ( ! me->lastdown ) {
            // trigger engaged
            me->lastdown = 1;
            sf_seek(me->snd, 0, SEEK_SET);
        }
    } else {
        if ( me->lastdown ) {
            me->lastdown = 0;
        }
    }

    if ( me->bufferat >= me->bufferused ) {
        me->bufferused = sf_read_float(me->snd, me->buffer, INPUT_SNDFILE_BUFFERSIZE);
        me->bufferat = 0;
    }

    me->now = me->buffer[me->bufferat + me->channel - 1];
    me->bufferat += me->channelcount;
}

float * input_sndfile_make(float *trigger, char *path, int channel, int startnow) {
    struct input_sndfile_st *ret;
    SF_INFO sfinfo;

    if ( (ret = malloc(sizeof(*ret))) == NULL )
        die("input_sndfile_make: couldn't malloc ret");

    if ( channel < 1 )
        die("input_sndfile_make: channel is less than one.");

    ret->now = 0;
    ret->bufferused = 0;
    ret->bufferat = 0;
    ret->channel = channel;
    ret->trigger = trigger;
    if ( (ret->snd = sf_open(path, SFM_READ, &sfinfo)) == NULL )
        diem("input_sndfile_make: couldn't open sndfile", path);

    ret->channelcount = sfinfo.channels;

    if ( channel > sfinfo.channels )
        diem("input_sndfile_make: the file doesn't have that many channels", path);

    if ( ! sfinfo.seekable )
        diem("input_sndfile_make: input file is not seekable", path);

    // make sure we don't play right away unless triggered or told to
    if ( ! startnow )
        sf_seek(ret->snd, 0, SEEK_END);

    ii_sampler_call(input_sndfile_ticker, (void *)ret);

    return &ret->now;
}

