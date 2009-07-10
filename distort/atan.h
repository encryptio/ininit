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

