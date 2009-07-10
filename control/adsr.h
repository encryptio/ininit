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
/* !doc:control_adsr(trigger, attack, decay, sustain, release)
 *      Returns a signal representing the ADSR envelope for the given
 *      arguments. Attack, decay, and release are measured in seconds. These
 *      measurements are measured as the time it takes to go from zero to one,
 *      not the time it takes for the actual given change. Sustain is the
 *      value in (0,1] that you decay to and hold at while the trigger is
 *      down.
 *
 *      Note: If the trigger is released before the attack happens, it does
 *      not finish the attack to 1.0 but immediately goes into decay. If your
 *      trigger is only one sample long, the envelope will probably be very,
 *      very low level.
 */
float * control_adsr_make(float *trigger, float *attack, float *decay, float *sustain, float *release);

#endif

