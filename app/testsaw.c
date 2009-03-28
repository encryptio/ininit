// Copyright 2009 Jack Christopher Kastorff

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

