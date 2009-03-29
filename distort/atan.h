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
/* !doc:distort_atan(input, amplitude)
 *      Adds a distortion effect based on the atan() function. The input is
 *      multiplied by amplitude before going into this function.
 */
float * distort_atan_make(float *input, float *amplitude);

#endif

