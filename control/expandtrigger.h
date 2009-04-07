// Copyright 2009 Jack Christopher Kastorff

#ifndef _CONTROL_EXPANDTRIGGER_H_
#define _CONTROL_EXPANDTRIGGER_H_

struct control_expandtrigger_st {
    float now;
    int lastmark;
    int triggered;
    float *length;
    float *input;
};

// !lua:control_expandtrigger -> control_expandtrigger_make(float *input, float *length)
/* !doc:control_expandtrigger(input, length)
 *      Takes the given input trigger and expands its pulses (forward in time)
 *      to length seconds. Often useful in conjunction with control_totrigger.
 */
float * control_expandtrigger_make(float *input, float *length);

#endif

