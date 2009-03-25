// Copyright 2009 Jack Christopher Kastorff

#include <stdlib.h>

#include "osc/white.h"
#include "die.h"
#include "helpers.h"

void osc_white_ticker(void * info) {
    struct osc_white_st * me = (struct osc_white_st *)info;

    me->now = ((float)rand()) / RAND_MAX;
}

struct osc_white_st * osc_white_make() {
    struct osc_white_st * ret;

    if ( (ret = malloc(sizeof(*ret))) == NULL )
        die("osc_white_make: couldn't malloc");

    ii_sampler_call(osc_white_ticker, (void *)ret);

    return ret;
}

