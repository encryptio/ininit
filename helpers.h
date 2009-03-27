// Copyright 2009 Jack Christopher Kastorff

#ifndef _HELPERS_H_
#define _HELPERS_H_

long long *current_sample;
long long *sample_rate;

// the only form of callback
typedef void (*pullfn)(void *info);

// initialize the helper library - call this before anything else
// note: memory leaks can occur if ii_init is called more than once
void ii_init(void);

// adds a callback to be called for every sample
void ii_sampler_call(pullfn sampler, void *info);

// adds a callback to be called every frequency samples
void ii_control_call(pullfn control, void *info, int frequency);

// runs *dst = *src for every sample
void ii_sampler_move(float *src, float *dst);

// run for a certain number of samples
void ii_run(int samples);

#endif
