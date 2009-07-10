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
#include "osc/sawtooth.h"
#include "filter/bandpass.h"
#include "saver.h"

// constant signals
float freq = 440;
float filterfreq = 2000;
float filterres = 0.999;

int main(int argc, char **argv) {
    float *saw, *filtered;

    ii_init();

    saw = osc_sawtooth_make(0, &freq);
    filtered = filter_bandpass_make(saw, &filterfreq, &filterres);

    saver_make_single(filtered, "audio/testsaw.au");

    ii_run(*sample_rate * 3);

    exit(0);
}

