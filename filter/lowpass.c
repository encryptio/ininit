// Copyright 2009 Jack Christopher Kastorff

#include <stdlib.h>
#include <math.h>

#define PI 3.141592653589793238462643383

#include "filter/lowpass.h"
#include "helpers.h"
#include "die.h"

void filter_lowpass_ticker(void * info) {
    struct filter_lowpass_st *me = (struct filter_lowpass_st *)info;
    
    if ( *(me->freq) != me->lastfreq )
        me->c = 2 - 2 * cosf(2*PI * *me->freq / *sample_rate);

    me->speed += (*me->input - me->pos) * me->c;
    me->pos += me->speed;
    me->speed *= *me->res;
}

float * filter_lowpass_make(float *input, float *frequency, float *resonance) {
    struct filter_lowpass_st *ret;

    if ( (ret = malloc(sizeof(*ret))) == NULL )
        die("filter_lowpass_st: couldn't malloc ret");

    ret->now = 0;
    ret->lastfreq = -1;
    ret->c = 0;
    ret->pos = 0;
    ret->speed = 0;
    ret->input = input;
    ret->freq = frequency;
    ret->res = resonance;

    ii_sampler_move(&(ret->pos), &(ret->now));
    ii_sampler_call(filter_lowpass_ticker, (void *)ret);

    return &ret->now;
}

