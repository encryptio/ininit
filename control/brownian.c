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

#include "control/brownian.h"
#include "die.h"
#include "helpers.h"

void control_brownian_ticker(void * info) {
    struct control_brownian_st * me = (struct control_brownian_st *)info;
    double new, mark;

    // brown noise
    new = me->now + ((double)rand()) / RAND_MAX - 0.5;

    // apply rc lowpass filter
    new = me->now + *me->filterat * (me->now - new);

    // apply stupid highpass filter
    new *= (1 - *me->filterat * 0.02);

    me->fnow = (double)(me->now = new) / (*me->filterat + 0.0005) * 0.02;
}

float * control_brownian_make(float *filterat) {
    struct control_brownian_st * ret;

    if ( (ret = malloc(sizeof(*ret))) == NULL )
        die("control_brownian_make: couldn't malloc");

    ret->filterat = filterat;
    ret->fnow = 0;
    ret->now = 0;

    ii_sampler_call(control_brownian_ticker, (void *)ret);

    return &ret->fnow;
}

