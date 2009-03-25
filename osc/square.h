// Copyright 2009 Jack Christopher Kastorff

#ifndef _OSC_SQUARE_H_
#define _OSC_SQUARE_H_

struct osc_square_st {
    float now;
    float phase;
    float *width;
    float *frequency;
};

// !lua:osc_square -> osc_square_make(float phase, float *frequency, float *width)
struct osc_square_st * osc_square_make(float phase, float *frequency, float *width);

#endif

