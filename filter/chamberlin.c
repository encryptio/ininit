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
#include <math.h>

#define PI 3.141592653589793238462643383

#include "filter/chamberlin.h"
#include "helpers.h"
#include "die.h"

void filter_chamberlin_ticker(void * info) {
    struct filter_chamberlin_st *me = (struct filter_chamberlin_st *)info;
    float f, high, notch;
    int i;
    
    if ( me->lastqvalue != *me->qvalue ) {
        me->q = sqrtf(1 - atanf(sqrtf(*me->qvalue)) * 2 / PI);
        me->scale = sqrtf(me->q);
    }

    f = *me->frequency / *sample_rate * 2;

    // oversampled 2x to allow frequencies up to sr/2 instead of sr/4
    for (i = 0; i<2; i++) {
        me->low += f * me->band;
        high = me->scale * *me->input - me->low - me->q * me->band;
        me->band += f * high;
    }

    notch = high + me->low;

    me->next =
          *me->mixdry   * *me->input
        + *me->mixlow   * me->low
        + *me->mixhigh  * high
        + *me->mixband  * me->band
        + *me->mixnotch * notch;
}

float * filter_chamberlin_make(float *input, float *frequency, float *qvalue, float *mixdry, float *mixlow, float *mixhigh, float *mixband, float *mixnotch) {
    struct filter_chamberlin_st *ret;

    if ( (ret = malloc(sizeof(*ret))) == NULL )
        die("filter_chamberlin_st: couldn't malloc ret");

    ret->now = 0;
    ret->next = 0;

    ret->q = 0;
    ret->scale = 0;
    ret->lastqvalue = -1;

    ret->low = 0;
    ret->band = 0;

    ret->input = input;
    ret->frequency = frequency;
    ret->qvalue = qvalue;
    ret->mixdry = mixdry;
    ret->mixlow = mixlow;
    ret->mixhigh = mixhigh;
    ret->mixband = mixband;
    ret->mixnotch = mixnotch;

    ii_sampler_move(&(ret->next), &(ret->now));
    ii_sampler_call(filter_chamberlin_ticker, (void *)ret);

    return &ret->now;
}

