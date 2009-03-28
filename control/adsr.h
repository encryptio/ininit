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
    float now;
    enum control_adsr_state state;
    float *attack;
    float *decay;
    float *sustain;
    float *release;
    float *trigger;
};

// !lua:control_adsr -> control_adsr_make(float *trigger, float *attack, float *decay, float *sustain, float *release)
float * control_adsr_make(float *trigger, float *attack, float *decay, float *sustain, float *release);

#endif

