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

#include "filter/delay.h"
#include "helpers.h"
#include "die.h"

#define BUFFER_OVERSIZE 500
#define BUFFER_MAX_FILLED 0.5

#define GETBUFFER(me,i) ((i) > me->buffersize ? 0 : (i) < me->bufferpos ? me->buffer[me->bufferpos-(i)-1] : me->buffer[me->buffersize-(i)+me->bufferpos-1])

void filter_delay_ticker(void * info) {
    struct filter_delay_st *me = (struct filter_delay_st *)info;
    int i, newlen, di;
    float df, y0, y1, y2, y3, a0, a1, a2, a3, df2;
    float *newbuf;
    
    if ( *me->length * *sample_rate > me->buffersize * BUFFER_MAX_FILLED ) {
        // need to make more space.

        newlen = *me->length * *sample_rate / BUFFER_MAX_FILLED + BUFFER_OVERSIZE;

        if ( (newbuf = malloc(sizeof(float) * newlen)) == NULL )
            die("filter_delay_ticker: couldn't malloc new buffer");

        // copy the contents of the circular buffer, aligned so that the bufferpos is at 0
        for (i=me->bufferpos; i<me->buffersize; i++)
            newbuf[i-me->bufferpos] = me->buffer[i];
        for (i=0; i<me->bufferpos; i++)
            newbuf[i+me->bufferpos] = me->buffer[i];

        // and fill the rest with zeroes
        for (i=me->buffersize; i<newlen; i++)
            newbuf[i] = 0;

        free(me->buffer);

        me->buffersize = newlen;
        me->buffer = newbuf;
        me->bufferpos = 0;
    }

    // fractional delay interpolated with a cubic spline
    di = (int) (*me->length * *sample_rate);
    df = *me->length * *sample_rate - di;

    y0 = GETBUFFER(me,di-1);
    y1 = GETBUFFER(me,di  );
    y2 = GETBUFFER(me,di+1);
    y3 = GETBUFFER(me,di+2);

    a0 = y3 - y2 - y0 + y1;
    a1 = y0 - y1 - a0;
    a2 = y2 - y0;
    a3 = y1;

    df2 = df*df;

    me->now = a0*df*df2 + a1*df2 + a2*df + a3;

    me->buffer[me->bufferpos++] = *me->feedback * me->now + *me->input;
    if ( me->bufferpos == me->buffersize )
        me->bufferpos = 0;
}

float * filter_delay_make(float *input, float *length, float *feedback) {
    struct filter_delay_st *ret;
    int i;

    if ( (ret = malloc(sizeof(*ret))) == NULL )
        die("filter_delay_make: couldn't malloc ret");

    ret->now = 0;
    if ( (ret->buffer = malloc(sizeof(float) * BUFFER_OVERSIZE)) == NULL )
        die("filter_delay_make: couldn't malloc initial buffer");
    ret->buffersize = BUFFER_OVERSIZE;
    ret->bufferpos = 0;
    ret->input = input;
    ret->length = length;
    ret->feedback = feedback;

    for (i=0; i<ret->buffersize; i++)
        ret->buffer[i] = 0;

    ii_sampler_call(filter_delay_ticker, (void *)ret);

    return &ret->now;
}

