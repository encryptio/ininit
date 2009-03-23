// Copyright 2009 Jack Christopher Kastorff

#ifndef _FILTER_BANDPASS_H_
#define _FILTER_BANDPASS_H_

struct filter_bandpass_st {
    double now;
    double next;
    double lastfreq;
    double lastres;
    double fx;
    double a0;
    double last1;
    double last2;
    double *input;
    double *freq;
    double *res;
};

struct filter_bandpass_st * filter_bandpass_make(double *input, double *frequency, double *resonance);

#endif

