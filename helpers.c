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

#include <stdlib.h>

#include "helpers.h"
#include "die.h"

// TODO: make these dynamically resized
#ifndef MAX_FUNCTIONS
#define MAX_FUNCTIONS 5000
#endif

static struct { pullfn fn; void *info; } funs[MAX_FUNCTIONS];
static int numfns = 0;

static struct { pullfn fn; void *info; int freq; } ctrl[MAX_FUNCTIONS];
static int numctrl = 0;

static struct { float *src, *dst; } move[MAX_FUNCTIONS];
static int nummove = 0;

static struct { pullfn fn; void *info; } death[MAX_FUNCTIONS];
static int numdeath = 0;

void ii_init(void) {
    if ( (current_sample = malloc(sizeof(*current_sample))) == NULL )
        die("ii_init: couldn't malloc space for current_sample");
    *current_sample = 0;

    if ( (sample_rate = malloc(sizeof(*sample_rate))) == NULL )
        die("ii_init: couldn't malloc space for sample_rate");
    *sample_rate = 44100;

    numfns = 0;
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

void ii_control_call(pullfn control, void *info, int frequency) {
    if ( numctrl == MAX_FUNCTIONS )
        die("ii_control_call: too many functions");

    ctrl[numctrl].fn = control;
    ctrl[numctrl].info = info;
    ctrl[numctrl].freq = frequency;
    numctrl++;
}

void ii_sampler_move(float *src, float *dst) {
    if ( nummove == MAX_FUNCTIONS )
        die("ii_sampler_move: too many moves");

    move[nummove].src = src;
    move[nummove].dst = dst;
    nummove++;
}

void ii_death_call(pullfn fn, void *info) {
    if ( numdeath == MAX_FUNCTIONS )
        die("ii_death_call: too many functions");

    death[numdeath].fn = fn;
    death[numdeath].info = info;
    numdeath++;
}

void ii_run(int samples) {
    int i, j;

    for (i=0; i<samples; i++) {
        for (j=0; j<numfns; j++)
            (funs[j].fn)(funs[j].info);

        for (j=0; j<numctrl; j++) {
            if ( *current_sample % ctrl[j].freq == 0 )
                (ctrl[j].fn)(ctrl[j].info);
        }

        for (j=0; j<nummove; j++)
            *(move[j].dst) = *(move[j].src);

        (*current_sample)++;
    }
}

void ii_death(void) {
    int i;

    for (i=0; i<numdeath; i++)
        (death[i].fn)(death[i].info);
}

