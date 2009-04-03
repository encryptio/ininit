// Copyright 2009 Jack Christopher Kastorff

#include <stdlib.h>
#include <math.h>

#define PI 3.141592653589793238462643383

#include "filter/delay.h"
#include "helpers.h"
#include "die.h"

#define BUFFER_OVERSIZE 200
#define BUFFER_MAX_FILLED 0.75

void filter_delay_ticker(void * info) {
    struct filter_delay_st *me = (struct filter_delay_st *)info;
    int i, newlen, di;
    float df, y0, y1, y2, y3, a0, a1, a2, a3, df2;
    
    if ( *me->length * *sample_rate > me->buffersize * BUFFER_MAX_FILLED ) {
        // need to make more space.

        newlen = *me->length * *sample_rate / BUFFER_MAX_FILLED + BUFFER_OVERSIZE;

        if ( (me->buffer = realloc(me->buffer, sizeof(float)*newlen)) == NULL )
            die("filter_delay_ticker: couldn't realloc buffer");

        for (i=me->buffersize; i<newlen; i++)
            me->buffer[i] = 0;

        me->buffersize = newlen;
    }

    // fractional delay interpolated with a cubic spline
    di = (int) (*me->length * *sample_rate);
    df = *me->length * *sample_rate - di;

    y0 = (di-1 < 0 || di-1 >= me->buffersize) ? 0 : me->buffer[di-1];
    y1 = (di   < 0 || di   >= me->buffersize) ? 0 : me->buffer[di  ];
    y2 = (di+1 < 0 || di+1 >= me->buffersize) ? 0 : me->buffer[di+1];
    y3 = (di+2 < 0 || di+2 >= me->buffersize) ? 0 : me->buffer[di+2];

    a0 = y3 - y2 - y0 + y1;
    a1 = y0 - y1 - a0;
    a2 = y2 - y0;
    a3 = y1;

    df2 = df*df;

    me->now = a0*df*df2 + a1*df2 + a2*df + a3;

    // TODO: circular buffer for better performance
    for (i=me->buffersize; i>0; i--)
        me->buffer[i] = me->buffer[i-1];
    me->buffer[0] = *me->feedback * me->now + *me->input;
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
    ret->input = input;
    ret->length = length;
    ret->feedback = feedback;

    for (i=0; i<ret->buffersize; i++)
        ret->buffer[i] = 0;

    ii_sampler_call(filter_delay_ticker, (void *)ret);

    return &ret->now;
}

