// Copyright 2009 Jack Christopher Kastorff

#ifndef _FILTER_BANDPASS_H_
#define _FILTER_BANDPASS_H_

struct filter_bandpass_st {
    float now;
    float next;
    float lastfreq;
    float lastres;
    float fx;
    float a0;
    float last1;
    float last2;
    float *input;
    float *freq;
    float *res;
};

// !lua:filter_bandpass -> filter_bandpass_make(float *input, float *frequency, float *resonance)
struct filter_bandpass_st * filter_bandpass_make(float *input, float *frequency, float *resonance);

#endif

