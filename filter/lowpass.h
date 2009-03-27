// Copyright 2009 Jack Christopher Kastorff

#ifndef _FILTER_LOWPASS_H_
#define _FILTER_LOWPASS_H_

struct filter_lowpass_st {
    float now;
    float lastfreq;
    float c;
    float pos; // == next
    float speed;
    float *input;
    float *freq;
    float *res;
};

// !lua:filter_lowpass -> filter_lowpass_make(float *input, float *frequency, float *resonance)
struct filter_lowpass_st * filter_lowpass_make(float *input, float *frequency, float *resonance);

#endif

