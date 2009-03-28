// Copyright 2009 Jack Christopher Kastorff

#include <stdlib.h>
#include <math.h>

#include "control/envelopefollower.h"
#include "helpers.h"
#include "die.h"

#define ENVELOPE_DROP 0.003
#define LOWPASS_ALPHA 0.06

void control_envelopefollower_ticker(void * info) {
    struct control_envelopefollower_st *me = (struct control_envelopefollower_st *)info;
    float new = me->now * (1.0-ENVELOPE_DROP);

    if ( fabsf(*me->input) > new )
        new = fabsf(*me->input);

    me->now = me->now + LOWPASS_ALPHA * (new - me->now);
}

struct control_envelopefollower_st * control_envelopefollower_make(float *input) {
    struct control_envelopefollower_st *ret;

    if ( (ret = malloc(sizeof(*ret))) == NULL )
        die("control_envelopefollower_st: couldn't malloc ret");

    ret->now = 0;
    ret->input = input;

    ii_sampler_call(control_envelopefollower_ticker, (void *)ret);

    return ret;
}

