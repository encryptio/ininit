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

#include "osc/square.h"
#include "die.h"
#include "helpers.h"

void osc_square_ticker(void * info) {
    struct osc_square_st * me = (struct osc_square_st *)info;

    me->phase += fabs(*me->frequency) / *sample_rate;
    if ( me->phase > 1 ) me->phase -= 1;

    me->now = me->phase > *me->width ? 1 : -1;
}

float * osc_square_make(float phase, float *frequency, float *width) {
    struct osc_square_st * ret;

    if ( (ret = malloc(sizeof(*ret))) == NULL )
        die("osc_square_make: couldn't malloc");

    ret->now = 0;
    ret->phase = phase;
    ret->width = width;
    ret->frequency = frequency;

    ii_sampler_call(osc_square_ticker, (void *)ret);

    return &ret->now;
}

