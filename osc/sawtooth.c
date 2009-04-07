// Copyright 2009 Jack Christopher Kastorff

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

