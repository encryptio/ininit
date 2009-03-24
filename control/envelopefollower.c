// Copyright 2009 Jack Christopher Kastorff

#include <stdlib.h>
#include <math.h>

#include "control/envelopefollower.h"
#include "helpers.h"
#include "die.h"

void control_envelopefollower_ticker(void * info) {
    struct control_envelopefollower_st *me = (struct control_envelopefollower_st *)info;

    me->now *= me->mult;
    
    if ( fabs(*me->input) > me->now )
        me->now = fabs(*me->input);
}

struct control_envelopefollower_st * control_envelopefollower_make(float *input, float drop) {
    struct control_envelopefollower_st *ret;

    if ( (ret = malloc(sizeof(*ret))) == NULL )
        die("control_envelopefollower_st: couldn't malloc ret");

    ret->now = 0;
    ret->mult = 1-drop;
    ret->input = input;

    ii_sampler_call(control_envelopefollower_ticker, (void *)ret);

    return ret;
}

