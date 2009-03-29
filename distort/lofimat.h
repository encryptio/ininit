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
/* !doc:distort_lofimat(input, bits, rate)
 *      Distorts the input signal by taking a value at the given rate (in hz),
 *      and quantizing it to the number of bits (valid in [-1:1]).
 */
float * distort_lofimat_make(float *input, float *bits, float *rate);

#endif

