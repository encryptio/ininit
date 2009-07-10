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

#ifndef _FILTER_DELAY_H_
#define _FILTER_DELAY_H_

struct filter_delay_st {
    float now;
    float *buffer;
    int buffersize;
    int bufferpos;
    float *input;
    float *length;
    float *feedback;
};

// !lua:filter_delay -> filter_delay_make(float *input, float *length, float *feedback)
/* !doc:filter_delay(input, length, feedback)
 *      Delays the given input by length seconds. The next input to the delay
 *      line is input + output*feedback.
 */
float * filter_delay_make(float *input, float *length, float *feedback);

#endif

