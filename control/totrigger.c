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

#include "control/totrigger.h"
#include "die.h"
#include "helpers.h"

void control_totrigger_ticker(void * info) {
    struct control_totrigger_st * me = (struct control_totrigger_st *)info;

    if ( fabs(*me->input) > 0.51 && me->state == control_totrigger_zeroed ) {
        me->state = control_totrigger_mark;
    } else if ( fabs(*me->input) > 0.49 && (me->state == control_totrigger_mark || me->state == control_totrigger_hold) ) {
        me->state = control_totrigger_hold;
    } else {
        me->state = control_totrigger_zeroed;
    }

    me->now = me->state == control_totrigger_mark ? 1 : 0;
}

float * control_totrigger_make(float *input) {
    struct control_totrigger_st * ret;

    if ( (ret = malloc(sizeof(*ret))) == NULL )
        die("control_totrigger_make: couldn't malloc");

    ret->now = 0;
    ret->input = input;
    ret->state = control_totrigger_zeroed;

    ii_sampler_call(control_totrigger_ticker, (void *)ret);

    return &ret->now;
}

