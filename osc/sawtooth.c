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

#include "osc/sawtooth.h"
#include "die.h"
#include "helpers.h"

void osc_sawtooth_ticker(void * info) {
    struct osc_sawtooth_st * me = (struct osc_sawtooth_st *)info;

    me->now += fabs(*me->frequency) / *sample_rate * 2;
    if ( me->now > 1 ) me->now -= 2;
}

float * osc_sawtooth_make(float phase, float *frequency) {
    struct osc_sawtooth_st * ret;

    if ( (ret = malloc(sizeof(*ret))) == NULL )
        die("osc_sawtooth_make: couldn't malloc");

    ret->now = phase;
    ret->frequency = frequency;

    ii_sampler_call(osc_sawtooth_ticker, (void *)ret);

    return &ret->now;
}

