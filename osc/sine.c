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

#include "osc/sine.h"
#include "die.h"
#include "helpers.h"

void osc_sine_ticker(void * info) {
    struct osc_sine_st * me = (struct osc_sine_st *)info;

    me->phase += fabs(*me->frequency) * PI * 2 / *sample_rate;
    if ( me->phase > PI*2 ) me->phase -= PI*2;

    me->now = cosf(me->phase);
}

float * osc_sine_make(float phase, float *frequency) {
    struct osc_sine_st * ret;

    if ( (ret = malloc(sizeof(*ret))) == NULL )
        die("osc_sine_make: couldn't malloc");

    ret->now = 0;
    ret->phase = phase * 2 * PI;
    ret->frequency = frequency;

    ii_sampler_call(osc_sine_ticker, (void *)ret);

    return &ret->now;
}

