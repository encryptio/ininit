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

#define PI 3.141592653589793238462643383

#include "distort/atan.h"
#include "helpers.h"
#include "die.h"

void distort_atan_ticker(void * info) {
    struct distort_atan_st *me = (struct distort_atan_st *)info;
    
    me->next = atanf( *(me->input) * *(me->amp) )/(PI/2);
}

float * distort_atan_make(float *input, float *amplitude) {
    struct distort_atan_st *ret;

    if ( (ret = malloc(sizeof(*ret))) == NULL )
        die("distort_atan_st: couldn't malloc ret");

    ret->now = 0;
    ret->next = 0;
    ret->input = input;
    ret->amp = amplitude;

    ii_sampler_move(&(ret->next), &(ret->now));
    ii_sampler_call(distort_atan_ticker, (void *)ret);

    return &ret->now;
}

