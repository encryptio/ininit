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

