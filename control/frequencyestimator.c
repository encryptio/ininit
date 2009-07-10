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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "control/frequencyestimator.h"
#include "helpers.h"
#include "die.h"

#define BUFFER_SIZE_SECONDS 0.017
#define RUNEVERY 40

void control_frequencyestimator_ticker(void * info) {
    struct control_frequencyestimator_st *me = (struct control_frequencyestimator_st *)info;
    int i;

    // TODO: optimize buffer with a circular buffer structure

    for (i=1; i<me->buffer_size; i++)
        me->buffer[i-1] = me->buffer[i];

    me->buffer[me->buffer_size-1] = *me->input;
}

void control_frequencyestimator_controlticker(void * info) {
    struct control_frequencyestimator_st *me = (struct control_frequencyestimator_st *)info;
    int bestpoint, i, j;
    float bestcost, thiscost, allcosts, cmndf;

    allcosts = 0;

    bestpoint = -1;
    bestcost = 0;

    // this implements the first three steps of the YIN F0 estimator
    // "YIN, a fundamental frequency estimator for speech and music"
    // -- 2002, Alain de Cheveigne and Hideki Kawahara

    for (i=0; i<me->buffer_size/2; i++) {
        thiscost = 0;
        for (j=0; j<me->buffer_size-i-1; j++) {
            thiscost += (me->buffer[j] - me->buffer[j+i]) * (me->buffer[j] - me->buffer[j+i]);
        }
        allcosts += thiscost;

        if ( i > 20 ) {
            cmndf = thiscost / allcosts * i;

            if ( bestpoint == -1 || bestcost > cmndf ) {
                bestpoint = i;
                bestcost = cmndf;
            }
        }
    }

    me->now = (float)(*sample_rate) / bestpoint; // frequency, in hz
}

float * control_frequencyestimator_make(float *input) {
    struct control_frequencyestimator_st *ret;

    if ( (ret = malloc(sizeof(*ret))) == NULL )
        die("control_frequencyestimator_make: couldn't malloc ret");

    ret->now = 0;
    ret->input = input;
    ret->buffer_size = (int) ceilf(BUFFER_SIZE_SECONDS * *sample_rate);

    if ( (ret->buffer = malloc(sizeof(float) * ret->buffer_size)) == NULL )
        die("control_frequencyestimator_make: couldn't malloc buffer");

    ii_sampler_call(control_frequencyestimator_ticker, (void *)ret);
    ii_control_call(control_frequencyestimator_controlticker, (void *)ret, RUNEVERY);

    return &ret->now;
}

