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

#include "filter/bandpass.h"
#include "helpers.h"
#include "die.h"

void filter_bandpass_ticker(void * info) {
    struct filter_bandpass_st *me = (struct filter_bandpass_st *)info;
    
    if ( *(me->freq) != me->lastfreq ) {
        me->fx = cosf(2*PI * *(me->freq) / ((float)(*sample_rate)));
        me->a0 = (1- *(me->res)) * sqrtf( *(me->res) * (*(me->res) - 4*me->fx*me->fx + 2) + 1 );
    } else if ( *(me->res) != me->lastres ) {
        me->a0 = (1- *(me->res)) * sqrtf( *(me->res) * (*(me->res) - 4*me->fx*me->fx + 2) + 1 );
    }

    me->next = me->a0 * *(me->input)
        + 2 * me->fx * *(me->res) * me->last1
        - *(me->res) * *(me->res) * me->last2;

    me->last2 = me->last1;
    me->last1 = me->next;
}

float * filter_bandpass_make(float *input, float *frequency, float *resonance) {
    struct filter_bandpass_st *ret;

    if ( (ret = malloc(sizeof(*ret))) == NULL )
        die("filter_bandpass_st: couldn't malloc ret");

    ret->now = 0;
    ret->next = 0;
    ret->lastfreq = -1;
    ret->lastres = -1;
    ret->fx = 0;
    ret->a0 = 0;
    ret->last1 = 0;
    ret->last2 = 0;
    ret->input = input;
    ret->freq = frequency;
    ret->res = resonance;

    ii_sampler_move(&(ret->next), &(ret->now));
    ii_sampler_call(filter_bandpass_ticker, (void *)ret);

    return &ret->now;
}

