// Copyright 2009 Jack Christopher Kastorff

#include <stdlib.h>

#include "osc/square.h"
#include "die.h"
#include "helpers.h"

void osc_square_ticker(void * info) {
    struct osc_square_st * me = (struct osc_square_st *)info;

    me->phase += *(me->frequency) / *sample_rate;
    if ( me->phase > 1 ) me->phase -= 1;

    me->now = me->phase > *me->width ? 1 : -1;
}

struct osc_square_st * osc_square_make(float phase, float *frequency, float *width) {
    struct osc_square_st * ret;

    if ( (ret = malloc(sizeof(*ret))) == NULL )
        die("osc_square_make: couldn't malloc");

    ret->now = 0;
    ret->phase = phase;
    ret->width = width;
    ret->frequency = frequency;

    ii_sampler_call(osc_square_ticker, (void *)ret);

    return ret;
}

