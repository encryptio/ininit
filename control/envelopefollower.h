// Copyright 2009 Jack Christopher Kastorff

#ifndef _CONTROL_ENVELOPEFOLLOWER_H_
#define _CONTROL_ENVELOPEFOLLOWER_H_

struct control_envelopefollower_st {
    double now;
    double mult;
    double *input;
};

// !lua:control_envelopefollower -> control_envelopefollower_make(double *input, double drop)
struct control_envelopefollower_st * control_envelopefollower_make(double *input, double drop);

#endif

