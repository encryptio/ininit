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

#include "osc/white.h"
#include "die.h"
#include "helpers.h"

void osc_white_ticker(void * info) {
    struct osc_white_st * me = (struct osc_white_st *)info;

    me->now = ((float)rand()) / RAND_MAX * 2 - 1;
}

float * osc_white_make() {
    struct osc_white_st * ret;

    if ( (ret = malloc(sizeof(*ret))) == NULL )
        die("osc_white_make: couldn't malloc");

    ii_sampler_call(osc_white_ticker, (void *)ret);

    return &ret->now;
}

