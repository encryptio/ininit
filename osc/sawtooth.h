// Copyright 2009 Jack Christopher Kastorff

#ifndef _OSC_SAWTOOTH_H_
#define _OSC_SAWTOOTH_H_

struct osc_sawtooth_st {
    float now;
    float *frequency;
};

// !lua:osc_sawtooth -> osc_sawtooth_make(float phase, float *frequency)
struct osc_sawtooth_st * osc_sawtooth_make(float phase, float *frequency);

#endif

