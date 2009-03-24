// Copyright 2009 Jack Christopher Kastorff

#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"
#include "osc/sawtooth.h"
#include "filter/bandpass.h"
#include "saver.h"

float freq = 440;
float filterfreq = 2000;
float filterres = 0.999;

int main(int argc, char **argv) {
    void *saw, *saver, *filtered;
    saw = (void *) osc_sawtooth_make(0, &freq);
    filtered = (void *) filter_bandpass_make((float *)saw, &filterfreq, &filterres);
    saver = (void *) saver_make((float *)filtered, "audio/testsaw.au");
    ii_run(sample_rate * 3);
    exit(0);
}

