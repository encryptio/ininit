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
/* !doc:control_totrigger(input)
 *      Returns a trigger signal based on the given input - whenever the
 *      absolute value of the input is greater than 0.5, it triggers a single
 *      sample at value 1 and then 0 until the next time the input goes from
 *      under 0.5 to over 0.5.
 *
 *      This type of signal is useful for triggering some other modules, such
 *      as input_sndfile.
 *
 *      Example:
 *
 *      trigger = control_totrigger( cycletable(0.1, { 1, 0 }) )
 *      loop = input_sndfile(trigger, "input.wav", 1, false)
 */
float * control_totrigger_make(float *input);

#endif

