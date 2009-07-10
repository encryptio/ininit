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

#ifndef _FILTER_LOWPASS_H_
#define _FILTER_LOWPASS_H_

struct filter_lowpass_st {
    float now;
    float lastfreq;
    float c;
    float pos; // == next
    float speed;
    float *input;
    float *freq;
    float *res;
};

// !lua:filter_lowpass -> filter_lowpass_make(float *input, float *frequency, float *resonance)
/* !doc:filter_lowpass(input, frequency, resonance)
 *      Lowpass filter on input with the given frequency and resonance
 *      in [0:1).
 */
float * filter_lowpass_make(float *input, float *frequency, float *resonance);

#endif

