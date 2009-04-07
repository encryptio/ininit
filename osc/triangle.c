// Copyright 2009 Jack Christopher Kastorff

#include <stdlib.h>
#include <math.h>

#include "osc/triangle.h"
#include "die.h"
#include "helpers.h"

void osc_triangle_ticker(void * info) {
    struct osc_triangle_st * me = (struct osc_triangle_st *)info;
    float min;

    me->phase += fabs(*me->frequency) / *sample_rate;
    if ( me->phase > 1 ) me->phase -= 1;

    if ( me->phase < *me->balance ) {
        // up portion
        me->now = me->phase / *me->balance * 2 - 1;
    } else {
        // down portion
        min = 1 - *me->balance;
        if ( min == 0 ) { // in the unlikely event of a water landing...
            me->now = -1;
        } else {
            me->now = - ((me->phase - *me->balance) / (1 - *me->balance) * 2 - 1);
        }
    }
}

float * osc_triangle_make(float phase, float *frequency, float *balance) {
    struct osc_triangle_st * ret;

    if ( (ret = malloc(sizeof(*ret))) == NULL )
        die("osc_triangle_make: couldn't malloc");

    ret->now = 0;
    ret->phase = phase;
    ret->balance = balance;
    ret->frequency = frequency;

    ii_sampler_call(osc_triangle_ticker, (void *)ret);

    return &ret->now;
}

