// Copyright 2009 Jack Christopher Kastorff

#ifndef _CONTROL_TOTRIGGER_H_
#define _CONTROL_TOTRIGGER_H_

enum control_totrigger_state {
    control_totrigger_zeroed,
    control_totrigger_mark,
    control_totrigger_hold,
};

struct control_totrigger_st {
    float now;
    float *input;
    enum control_totrigger_state state;
};

// !lua:control_totrigger -> control_totrigger_make(float *input)
float * control_totrigger_make(float *input);

#endif

