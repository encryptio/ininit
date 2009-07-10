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

#include "control/expandtrigger.h"
#include "die.h"
#include "helpers.h"

void control_expandtrigger_ticker(void * info) {
    struct control_expandtrigger_st * me = (struct control_expandtrigger_st *)info;

    if ( fabs(*me->input) > 0.5 ) {
        me->triggered = 1;
        me->lastmark = *current_sample;
    }

    if ( me->triggered )
        me->now = (*current_sample - me->lastmark) < *sample_rate * *me->length ? 1 : 0;
}

float * control_expandtrigger_make(float *input, float *length) {
    struct control_expandtrigger_st * ret;

    if ( (ret = malloc(sizeof(*ret))) == NULL )
        die("control_expandtrigger_make: couldn't malloc");

    ret->now = 0;
    ret->lastmark = 0;
    ret->triggered = 0;
    ret->input = input;
    ret->length = length;

    ii_sampler_call(control_expandtrigger_ticker, (void *)ret);

    return &ret->now;
}

