/*
 * Copyright (c) 2009 Jack Christopher Kastorff
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions, and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * The name Chris Kastorff may not be used to endorse or promote
 *       products derived from this software without specific prior written
 *       permission.
 */

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

