// Copyright 2009 Jack Christopher Kastorff

#ifndef _HELPERS_H_
#define _HELPERS_H_

static long long current_sample = 0;
static long long sample_rate = 44100;

typedef void (*pullfn)(void *info);

void ii_init(void);
void ii_sampler_call(pullfn sampler, void *info);
void ii_control_call(pullfn control, void *info, const int frequency);
void ii_sampler_move(double *src, double *dst);
void ii_run(int samples);

#endif
