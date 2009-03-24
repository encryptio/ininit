// Copyright 2009 Jack Christopher Kastorff

#ifndef _SAVER_H_
#define _SAVER_H_

#include <stdio.h>

#define SAVER_BUFFER_SIZE 1024

struct saver_st {
    FILE * fh;
    float *input;
    char buffer[SAVER_BUFFER_SIZE];
    int buffer_used;
};

struct saver_st * saver_make(float *input, char *path);

#endif
