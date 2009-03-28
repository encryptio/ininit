// Copyright 2009 Jack Christopher Kastorff

#ifndef _CONTROL_ENVELOPEFOLLOWER_H_
#define _CONTROL_ENVELOPEFOLLOWER_H_

struct control_envelopefollower_st {
    float now;
    float *input;
};

// !lua:control_envelopefollower -> control_envelopefollower_make(float *input)
float * control_envelopefollower_make(float *input);

#endif

