// Copyright 2009 Jack Christopher Kastorff

#ifndef _CONTROL_ADSR_H_
#define _CONTROL_ADSR_H_

enum control_adsr_state {
    control_adsr_off,
    control_adsr_attack,
    control_adsr_decay,
    control_adsr_sustain,
    control_adsr_release,
};

struct control_adsr_st {
    double now;
    enum control_adsr_state state;
    double *attack;
    double *decay;
    double *sustain;
    double *release;
    double *trigger;
};

struct control_adsr_st * control_adsr_make(double *trigger, double *attack, double *decay, double *sustain, double *release);

#endif

