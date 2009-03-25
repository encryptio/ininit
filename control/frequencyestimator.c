// Copyright 2009 Jack Christopher Kastorff

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "control/frequencyestimator.h"
#include "helpers.h"
#include "die.h"

#define BUFFER_SIZE_SECONDS 0.016
#define RUNEVERY 100

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
    float bestcost, thiscost;

    bestpoint = -1;
    bestcost = 0;

    for (i=20; i<me->buffer_size*0.4-2; i++) {
        thiscost = 0;
        for (j=0; j<me->buffer_size-i-1; j++) {
            thiscost += (me->buffer[j] - me->buffer[j+i]) * (me->buffer[j] - me->buffer[j+i]); // nearly YIN
        }
        if ( bestpoint == -1 || bestcost > thiscost ) {
            printf("new best, old point is %d, new point is %d with cost %f\n", bestpoint, bestpoint, bestcost);
            bestpoint = i;
            bestcost = thiscost;
        }
    }

    me->now = (float)(*sample_rate) / bestpoint; // frequency, in hz

    printf("set to %f, bestpoint is %d with cost %f\n", me->now, bestpoint, bestcost);
}

struct control_frequencyestimator_st * control_frequencyestimator_make(float *input) {
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

    return ret;
}

