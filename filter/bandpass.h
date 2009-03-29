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
/* !doc:filter_bandpass(input, frequency, resonance)
 *      Filters the given input with a bandpass around the given frequency.
 *      The resonance parameter is [0:1), higher values mean a tighter band.
 *      In all cases, the gain at the given frequency is 0dB.
 */
float * filter_bandpass_make(float *input, float *frequency, float *resonance);

#endif

