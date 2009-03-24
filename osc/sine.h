// Copyright 2009 Jack Christopher Kastorff

#ifndef _OSC_SINE_H_
#define _OSC_SINE_H_

struct osc_sine_st {
    float now;
    float phase;
    float *frequency;
};

// !lua:osc_sine -> osc_sine_make(float phase, float *frequency)
struct osc_sine_st * osc_sine_make(float phase, float *frequency);

#endif

