// Copyright 2009 Jack Christopher Kastorff

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "saver.h"
#include "die.h"
#include "helpers.h"


void saver_ticker(void * info) {
    struct saver_st *me = (struct saver_st *)info;
    int val, i;
    
    if ( me->buffer_used + 2*me->num_inputs > SAVER_BUFFER_SIZE ) {
        fwrite(&(me->buffer), me->buffer_used, 1, me->fh);
        me->buffer_used = 0;
    }

    for (i=0; i<me->num_inputs; i++) {
        val = *(me->inputs[i]) * 32768.0;

        // clip
        if ( val >  (1 << 15) - 1 ) val =  (1 << 15) - 1;
        if ( val < -(1 << 15)     ) val = -(1 << 15);

        // signed mark
        if ( val < 0 ) val += 1 << 16;

        me->buffer[me->buffer_used++] = (val >> 8) & 0xFF;
        me->buffer[me->buffer_used++] =  val       & 0xFF;
    }
}

void saver_death(void * info) {
    struct saver_st *me = (struct saver_st *)info;

    if ( me->buffer_used ) {
        fwrite(&(me->buffer), me->buffer_used, 1, me->fh);
    }

    fclose(me->fh);
}

void saver_make(float **inputs, int num_inputs, char *path) {
    struct saver_st *ret;
    char header[4 + 5*4] = {
        '.', 's', 'n', 'd', // magic number
        0, 0, 0, 24, // data starts at offset 24
        255, 255, 255, 255, // unknown length
        0, 0, 0, 3, // 16-bit linear pcm
        0, 0, 0, 0, // sample rate, filled later
        0, 0, 0, 0, // channel count, filled later
    };

    if ( (ret = malloc(sizeof(*ret))) == NULL )
        die("saver_make: couldn't malloc ret");

    if ( (ret->fh = fopen(path, "wb")) == NULL )
        die("saver_make: couldn't open path for writing");

    // write the header
    // sample rate, big-endian
    header[16] = ((*sample_rate) >> 24) & 0xFF;
    header[17] = ((*sample_rate) >> 16) & 0xFF;
    header[18] = ((*sample_rate) >> 8 ) & 0xFF;
    header[19] =  (*sample_rate)        & 0xFF;

    // number of inputs, big-endian
    header[5*4    ] = (num_inputs >> 24) & 0xFF;
    header[5*4 + 1] = (num_inputs >> 16) & 0xFF;
    header[5*4 + 2] = (num_inputs >>  8) & 0xFF;
    header[5*4 + 3] =  num_inputs        & 0xFF;

    fwrite(header, 4 + 5*4, 1, ret->fh);

    // variables
    ret->buffer_used = 0;

    // and the inputs
    ret->inputs = inputs;
    ret->num_inputs = num_inputs;

    // and set up the sampler
    ii_sampler_call(saver_ticker, (void *)ret);
    ii_death_call(saver_death, (void *)ret);
}

void saver_make_single(float *input, char *path) {
    float **inputs;

    if ( (inputs = malloc(sizeof(float *))) == NULL )
        die("saver_make_single: couldn't malloc inputs");

    inputs[0] = input;

    saver_make(inputs, 1, path);
}

