// Copyright 2009 Jack Christopher Kastorff

#ifndef _DISTORT_LOFIMAT_H_
#define _DISTORT_LOFIMAT_H_

struct distort_lofimat_st {
    float now;
    float next;
    double phase;
    float *bits;
    float *rate;
    float *input;
};

// !lua:distort_lofimat -> distort_lofimat_make(float *input, float *bits, float *rate)
float * distort_lofimat_make(float *input, float *bits, float *rate);

#endif

