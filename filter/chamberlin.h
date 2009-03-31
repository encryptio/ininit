// Copyright 2009 Jack Christopher Kastorff

#ifndef _FILTER_chamberlin_H_
#define _FILTER_chamberlin_H_

struct filter_chamberlin_st {
    float now;
    float next;

    float q;
    float scale;
    float lastqvalue;

    float low;
    float band;

    float *input;
    float *frequency;
    float *qvalue;
    float *mixdry;
    float *mixlow;
    float *mixhigh;
    float *mixband;
    float *mixnotch;
};

// !lua:filter_chamberlin -> filter_chamberlin_make(float *input, float *frequency, float *qvalue, float *mixdry, float *mixlow, float *mixhigh, float *mixband, float *mixnotch)
/* !doc:filter_chamberlin(input, frequency, q, mixdry, mixlow, mixhigh, mixband, mixnotch)
 *      A chamberlin two-pole filter. This module implements lowpass,
 *      highpass, bandpass, notch, highshelf, lowshelf, peaking, and dipping
 *      filters, as well as any linear combination of these.
 *
 *      This is a 12dB/oct filter.
 *
 *      The Q value (resonance) is [0,inf), but typical values range in
 *      [0,200]. Higher values give a higher resonance, and the overshoot
 *      caused by the resonance is automatically corrected by an appropriate
 *      gain on the input signal.
 *
 *      The mix* inputs are the linear amounts for which the given filter type
 *      is mixed in. They are all recommended in [0,1], but any value is
 *      acceptable.
 *
 *      All inputs are signals.
 *
 *      Examples:
 *
 *      -- lowpass filter
 *      filtered = filter_chamberlin(input, 3000, 20, 0,   1,   0,   0,   0)
 *
 *      -- highshelf filter
 *      filtered = filter_chamberlin(input, 3000, 20, 0.1, 0,   0.9, 0,   0)
 *
 *      -- peaking filter
 *      filtered = filter_chamberlin(input, 2300, 40, 0.3, 0,   0,   0.7, 0)
 */
float * filter_chamberlin_make(float *input, float *frequency, float *qvalue, float *mixdry, float *mixlow, float *mixhigh, float *mixband, float *mixnotch);

#endif

