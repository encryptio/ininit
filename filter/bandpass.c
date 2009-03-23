// Copyright 2009 Jack Christopher Kastorff

#include <stdlib.h>
#include <math.h>

#define PI 3.141592653589793238462643383

#include "filter/bandpass.h"
#include "helpers.h"
#include "die.h"

void filter_bandpass_ticker(void * info) {
    struct filter_bandpass_st *me = (struct filter_bandpass_st *)info;
    
    if ( *(me->freq) != me->lastfreq ) {
        me->fx = cos(2*PI * *(me->freq) / (double)sample_rate);
        me->a0 = (1- *(me->res)) * sqrt( *(me->res) * (*(me->res) - 4*me->fx*me->fx + 2) + 1 );
    } else if ( *(me->res) != me->lastres ) {
        me->a0 = (1- *(me->res)) * sqrt( *(me->res) * (*(me->res) - 4*me->fx*me->fx + 2) + 1 );
    }

    me->next = me->a0 * *(me->input)
        + 2 * me->fx * *(me->res) * me->last1
        - *(me->res) * *(me->res) * me->last2;

    me->last2 = me->last1;
    me->last1 = me->next;
}

struct filter_bandpass_st * filter_bandpass_make(double *input, double *frequency, double *resonance) {
    struct filter_bandpass_st *ret;

    if ( (ret = malloc(sizeof(*ret))) == NULL )
        die("filter_bandpass_st: couldn't malloc ret");

    ret->now = 0;
    ret->next = 0;
    ret->lastfreq = -1;
    ret->lastres = -1;
    ret->fx = 0;
    ret->a0 = 0;
    ret->last1 = 0;
    ret->last2 = 0;
    ret->input = input;
    ret->freq = frequency;
    ret->res = resonance;

    ii_sampler_move(&(ret->next), &(ret->now));
    ii_sampler_call(filter_bandpass_ticker, (void *)ret);

    return ret;
}

