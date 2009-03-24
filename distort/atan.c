// Copyright 2009 Jack Christopher Kastorff

#include <stdlib.h>
#include <math.h>

#define PI 3.141592653589793238462643383

#include "distort/atan.h"
#include "helpers.h"
#include "die.h"

void distort_atan_ticker(void * info) {
    struct distort_atan_st *me = (struct distort_atan_st *)info;
    
    me->next = atan( *(me->input) * *(me->amp) )/(PI/2);
}

struct distort_atan_st * distort_atan_make(float *input, float *amplitude) {
    struct distort_atan_st *ret;

    if ( (ret = malloc(sizeof(*ret))) == NULL )
        die("distort_atan_st: couldn't malloc ret");

    ret->now = 0;
    ret->next = 0;
    ret->input = input;
    ret->amp = amplitude;

    ii_sampler_move(&(ret->next), &(ret->now));
    ii_sampler_call(distort_atan_ticker, (void *)ret);

    return ret;
}

