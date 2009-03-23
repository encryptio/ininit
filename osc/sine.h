// Copyright 2009 Jack Christopher Kastorff

#ifndef _OSC_SINE_H_
#define _OSC_SINE_H_

struct osc_sine_st {
    double now;
    double phase;
    double *frequency;
};

struct osc_sine_st * osc_sine_make(double phase, double *frequency);

#endif

