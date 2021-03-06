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

#include "distort/lofimat.h"
#include "helpers.h"
#include "die.h"

void distort_lofimat_ticker(void * info) {
    struct distort_lofimat_st *me = (struct distort_lofimat_st *)info;
    int go = 0;
    float div;
    
    if ( *me->rate >= 0 ) {
        me->phase += ((double) *me->rate) / *sample_rate;
        if ( me->phase > 1 ) {
            me->phase -= 1;

            go = 1;
        }
    } else {
        go = 1;
    }

    if ( go ) {
        div = powf(2.0, *me->bits/2);
        me->next = roundf(*me->input*div)/div;
    }
}

float * distort_lofimat_make(float *input, float *bits, float *rate) {
    struct distort_lofimat_st *ret;

    if ( (ret = malloc(sizeof(*ret))) == NULL )
        die("distort_lofimat_st: couldn't malloc ret");

    ret->now = 0;
    ret->next = 0;
    ret->phase = 0;
    ret->input = input;
    ret->bits = bits;
    ret->rate = rate;

    ii_sampler_move(&(ret->next), &(ret->now));
    ii_sampler_call(distort_lofimat_ticker, (void *)ret);

    return &ret->now;
}

