// Copyright 2009 Jack Christopher Kastorff

#include "helpers.h"
#include "die.h"

#ifndef MAX_FUNCTIONS
#define MAX_FUNCTIONS 5000
#endif

struct { pullfn fn; void *info; } funs[MAX_FUNCTIONS];
int numfns = 0;

struct { pullfn fn; void *info; int freq; } ctrl[MAX_FUNCTIONS];
int numctrl = 0;

struct { double *src, *dst; } move[MAX_FUNCTIONS];
int nummove = 0;

void ii_init(void) {
    current_sample = 0;
    numfns = 0; // dealloc?
    numctrl = 0;
    nummove = 0;
}

void ii_sampler_call(pullfn sampler, void *info) {
    if ( numfns == MAX_FUNCTIONS )
        die("ii_sampler_call: too many functions");

    funs[numfns].fn = sampler;
    funs[numfns].info = info;
    numfns++;
}

void ii_control_call(pullfn control, void *info, const int frequency) {
    if ( numctrl == MAX_FUNCTIONS )
        die("ii_control_call: too many functions");

    ctrl[numctrl].fn = control;
    ctrl[numctrl].freq = frequency;
    numctrl++;
}

void ii_sampler_move(double *src, double *dst) {
    if ( nummove == MAX_FUNCTIONS )
        die("ii_sampler_move: too many moves");

    move[nummove].src = src;
    move[nummove].dst = dst;
    nummove++;
}

void ii_run(int samples) {
    int i, j;

    for (i=0; i<samples; i++) {
        for (j=0; j<numfns; j++)
            (funs[j].fn)(funs[j].info);

        for (j=0; j<numctrl; j++) {
            if ( ctrl[j].freq % current_sample == 0 )
                (ctrl[j].fn)(ctrl[j].info);
        }

        for (j=0; j<nummove; j++)
            *(move[j].dst) = *(move[j].src);

        current_sample++;
    }
}

