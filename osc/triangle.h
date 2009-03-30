// Copyright 2009 Jack Christopher Kastorff

#ifndef _OSC_TRIANGLE_H_
#define _OSC_TRIANGLE_H_

struct osc_triangle_st {
    float now;
    float phase;
    float *balance;
    float *frequency;
};

// !lua:osc_triangle -> osc_triangle_make(float phase, float *frequency, float *balance)
/* !doc:osc_triangle(phase, frequency, balance)
 *      Creates a triangle wave at the given frequency and starting at the
 *      given phase (in [0,1)). The balance parameter specifies how much of
 *      the sound is devoted to the "up" portion of the sound compared to the
 *      "down" portion. A value of 0 or 1 makes this a ramp/sawtooth
 *      oscillator.
 */
float * osc_triangle_make(float phase, float *frequency, float *balance);

#endif

