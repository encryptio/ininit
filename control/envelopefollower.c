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

#include "control/envelopefollower.h"
#include "helpers.h"
#include "die.h"

#define LOWPASS_ALPHA 0.06

void control_envelopefollower_ticker(void * info) {
    struct control_envelopefollower_st *me = (struct control_envelopefollower_st *)info;
    float new = me->now * me->drop;

    if ( fabsf(*me->input) > new )
        new = fabsf(*me->input);

    me->now = me->now + LOWPASS_ALPHA * (new - me->now);
}

float * control_envelopefollower_make(float *input, float drop) {
    struct control_envelopefollower_st *ret;

    if ( (ret = malloc(sizeof(*ret))) == NULL )
        die("control_envelopefollower_st: couldn't malloc ret");

    ret->now = 0;
    ret->drop = 1.0-drop;
    ret->input = input;

    ii_sampler_call(control_envelopefollower_ticker, (void *)ret);

    return &ret->now;
}

