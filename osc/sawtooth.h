// Copyright 2009 Jack Christopher Kastorff

#ifndef _OSC_SAWTOOTH_H_
#define _OSC_SAWTOOTH_H_

struct osc_sawtooth_st {
    double now;
    double *frequency;
};

struct osc_sawtooth_st * osc_sawtooth_make(double phase, double *frequency);

#endif

