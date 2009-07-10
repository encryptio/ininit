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

