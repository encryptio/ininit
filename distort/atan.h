// Copyright 2009 Jack Christopher Kastorff

#ifndef _DISTORT_ATAN_H_
#define _DISTORT_ATAN_H_

struct distort_atan_st {
    float now;
    float next;
    float *amp;
    float *input;
};

// !lua:distort_atan -> distort_atan_make(float *input, float *amplitude)
float * distort_atan_make(float *input, float *amplitude);

#endif

