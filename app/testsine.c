// Copyright 2009 Jack Christopher Kastorff

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

    freqwave = (float *) osc_sine_make(0, &freqwave_speed);
    ii_sampler_call(fn_ticker, NULL);

    mainwave = (float *) osc_sine_make(0, &freq);

    saver_make_single(mainwave, "audio/testsine.au");

    ii_run(*sample_rate * 10);

    exit(0);
}

