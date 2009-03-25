// Copyright 2009 Jack Christopher Kastorff

#ifndef _CONTROL_BROWNIAN_H_
#define _CONTROL_BROWNIAN_H_

struct control_brownian_st {
    float fnow;
    float *filterat;
    double now;
    double hp_last_out;
    double hp_last_in;
};

// !lua:control_brownian -> control_brownian_make(float *filterat)
struct control_brownian_st * control_brownian_make(float *filterat);

#endif

