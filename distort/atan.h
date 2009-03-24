// Copyright 2009 Jack Christopher Kastorff

#ifndef _DISTORT_ATAN_H_
#define _DISTORT_ATAN_H_

struct distort_atan_st {
    double now;
    double next;
    double *amp;
    double *input;
};

// !lua:distort_atan -> distort_atan_make(double *input, double *amplitude)
struct distort_atan_st * distort_atan_make(double *input, double *amplitude);

#endif

