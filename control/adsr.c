// Copyright 2009 Jack Christopher Kastorff

#include <stdlib.h>
#include <math.h>

#include "control/adsr.h"
#include "helpers.h"
#include "die.h"

void control_adsr_ticker(void * info) {
    struct control_adsr_st *me = (struct control_adsr_st *)info;
    
    if ( fabsf(*me->trigger) > 0.5 ) {
        // trigger down
        switch ( me->state ) {
            case control_adsr_off:
            case control_adsr_release:
                me->state = control_adsr_attack;
                break;

            case control_adsr_attack:
                if ( *me->attack == 0 ) {
                    me->now = 1;
                    me->state = control_adsr_decay;
                } else {
                    me->now += 1/(*me->attack * *sample_rate);
                    if ( me->now > 1 ) {
                        me->now = 1;
                        me->state = control_adsr_decay;
                    }
                }
                break;

            case control_adsr_decay:
                if ( *me->decay == 0 ) {
                    me->now = *me->sustain;
                    me->state = control_adsr_sustain;
                } else {
                    me->now -= 1/(*me->decay * *sample_rate);
                    if ( me->now < *me->sustain ) {
                        me->now = *me->sustain;
                        me->state = control_adsr_sustain;
                    }
                }
                break;

            case control_adsr_sustain:
                me->now = *me->sustain;
                break;
        }
    } else {
        // trigger up
        switch ( me->state ) {
            case control_adsr_off:
                // nothing
                break;

            case control_adsr_release:
                me->now -= 1/(*me->release * *sample_rate);
                if ( me->now < 0 ) {
                    me->now = 0;
                    me->state = control_adsr_off;
                }
                break;

            default:
                me->state = control_adsr_release;
        }
    }
}

float * control_adsr_make(float *trigger, float *attack, float *decay, float *sustain, float *release) {
    struct control_adsr_st *ret;

    if ( (ret = malloc(sizeof(*ret))) == NULL )
        die("control_adsr_st: couldn't malloc ret");

    ret->now = 0;
    ret->state = control_adsr_off;
    ret->attack = attack;
    ret->decay = decay;
    ret->sustain = sustain;
    ret->release = release;
    ret->trigger = trigger;

    ii_sampler_call(control_adsr_ticker, (void *)ret);

    return &ret->now;
}

