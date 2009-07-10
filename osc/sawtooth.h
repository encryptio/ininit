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

#ifndef _OSC_SAWTOOTH_H_
#define _OSC_SAWTOOTH_H_

struct osc_sawtooth_st {
    float now;
    float *frequency;
};

// !lua:osc_sawtooth -> osc_sawtooth_make(float phase, float *frequency)
/* !doc:osc_sawtooth(phase, frequency)
 *      Creates a sawtooth wave at the given frequency and a starting offset
 *      of phase (in [0:1))
 */
float * osc_sawtooth_make(float phase, float *frequency);

#endif

