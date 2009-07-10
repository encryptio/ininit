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

#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"
#include "osc/sine.h"
#include "saver.h"

// constant signals
float freqwave_speed = 0.2;

// fn_ticker applied to a signal requires some temporary space
float freq = 0;
float *freqwave;

void fn_ticker(void * info) {
    freq = *freqwave * 100 + 440;
}

int main(int argc, char **argv) {
    float *mainwave;

    ii_init();

    freqwave = osc_sine_make(0, &freqwave_speed);
    ii_sampler_call(fn_ticker, NULL);

    mainwave = osc_sine_make(0, &freq);

    saver_make_single(mainwave, "audio/testsine.au");

    ii_run(*sample_rate * 10);

    exit(0);
}

