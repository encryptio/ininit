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

#ifndef _SAVER_H_
#define _SAVER_H_

#include <stdio.h>

#define SAVER_BUFFER_SIZE 1024

struct saver_st {
    FILE * fh;
    float **inputs;
    int num_inputs;
    char buffer[SAVER_BUFFER_SIZE];
    int buffer_used;
};

void saver_make(float **inputs, int num_inputs, char *path);
void saver_make_single(float *input, char *path);

#endif
