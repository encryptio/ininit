// Copyright 2009 Jack Christopher Kastorff

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

