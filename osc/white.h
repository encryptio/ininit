// Copyright 2009 Jack Christopher Kastorff

#ifndef _OSC_WHITE_H_
#define _OSC_WHITE_H_

struct osc_white_st {
    float now;
};

// !lua:osc_white -> osc_white_make()
struct osc_white_st * osc_white_make();

#endif

