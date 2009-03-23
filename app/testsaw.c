// Copyright 2009 Jack Christopher Kastorff

#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"
#include "osc/sawtooth.h"
#include "filter/bandpass.h"
#include "saver.h"

double freq = 440;
double filterfreq = 2000;
double filterres = 0.999;

int main(int argc, char **argv) {
    void *saw, *saver, *filtered;
    saw = (void *) osc_sawtooth_make(0, &freq);
    filtered = (void *) filter_bandpass_make((double *)saw, &filterfreq, &filterres);
    saver = (void *) saver_make((double *)filtered, "audio/testsaw.au");
    ii_run(sample_rate * 3);
    exit(0);
}

