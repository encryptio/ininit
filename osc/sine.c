// Copyright 2009 Jack Christopher Kastorff

#include <stdlib.h>
#include <math.h>

#define PI 3.141592653589793238462643383

#include "osc/sine.h"
#include "die.h"
#include "helpers.h"

void osc_sine_ticker(void * info) {
    struct osc_sine_st * me = (struct osc_sine_st *)info;

    me->phase += *(me->frequency) * PI * 2 / sample_rate;
    if ( me->phase > PI*2 ) me->phase -= PI*2;

    me->now = cosf(me->phase);
}

struct osc_sine_st * osc_sine_make(float phase, float *frequency) {
    struct osc_sine_st * ret;

    if ( (ret = malloc(sizeof(*ret))) == NULL )
        die("osc_sine_make: couldn't malloc");

    ret->phase = phase * 2 * PI;
    ret->frequency = frequency;

    ii_sampler_call(osc_sine_ticker, (void *)ret);

    return ret;
}

