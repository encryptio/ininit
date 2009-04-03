// Copyright 2009 Jack Christopher Kastorff

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

