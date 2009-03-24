// Copyright 2009 Jack Christopher Kastorff

#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"
#include "osc/sine.h"
#include "saver.h"

float freq = 0;
float *freqwave;

void fn_ticker(void * info) {
    freq = *freqwave * 100 + 440;
}

float freqwave_speed = 0.2;

int main(int argc, char **argv) {
    void *mainwave, *saver;

    freqwave = (float *) osc_sine_make(0, &freqwave_speed);
    ii_sampler_call(fn_ticker, NULL);

    mainwave = (void *) osc_sine_make(0, &freq);

    saver = (void *) saver_make((float *)mainwave, "audio/testsine.au");

    ii_run(sample_rate * 10);

    exit(0);
}

