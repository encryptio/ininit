// Copyright 2009 Jack Christopher Kastorff

#ifndef _CONTROL_FREQUENCYESTIMATOR_H_
#define _CONTROL_FREQUENCYESTIMATOR_H_

struct control_frequencyestimator_st {
    float now;
    float *input;
    float *buffer;
    int buffer_size;
};

// !lua:control_frequencyestimator -> control_frequencyestimator_make(float *input)
/* !doc:control_frequencyestimator(input)
 *      Estimates the fundamental frequency of the given input. TODO: details
 */
float * control_frequencyestimator_make(float *input);

#endif

