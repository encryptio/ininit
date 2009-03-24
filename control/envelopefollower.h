// Copyright 2009 Jack Christopher Kastorff

#ifndef _CONTROL_ENVELOPEFOLLOWER_H_
#define _CONTROL_ENVELOPEFOLLOWER_H_

struct control_envelopefollower_st {
    float now;
    float mult;
    float *input;
};

// !lua:control_envelopefollower -> control_envelopefollower_make(float *input, float drop)
struct control_envelopefollower_st * control_envelopefollower_make(float *input, float drop);

#endif

