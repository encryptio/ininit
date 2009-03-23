// Copyright 2009 Jack Christopher Kastorff

#ifndef _SAVER_H_
#define _SAVER_H_

struct saver_st {
    FILE * fh;
    double *input;
};

struct saver_st * saver_make(double *input, char *path);

#endif
