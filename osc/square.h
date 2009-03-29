// Copyright 2009 Jack Christopher Kastorff

#ifndef _OSC_SQUARE_H_
#define _OSC_SQUARE_H_

struct osc_square_st {
    float now;
    float phase;
    float *width;
    float *frequency;
};

// !lua:osc_square -> osc_square_make(float phase, float *frequency, float *width)
/* !doc:osc_square(phase, frequency, width)
 *      Creates a pulse-width modulated signal at the given frequency and the
 *      given width, starting at the given phase (in [0:1)).
 */
float * osc_square_make(float phase, float *frequency, float *width);

#endif

