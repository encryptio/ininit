// Copyright 2009 Jack Christopher Kastorff

#ifndef _HELPERS_H_
#define _HELPERS_H_

long long *current_sample;
long long *sample_rate;

typedef void (*pullfn)(void *info);

void ii_init(void);
void ii_sampler_call(pullfn sampler, void *info);
void ii_control_call(pullfn control, void *info, int frequency);
void ii_sampler_move(float *src, float *dst);
void ii_run(int samples);

#endif
