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

#ifndef _HELPERS_H_
#define _HELPERS_H_

long long *current_sample;
long long *sample_rate;

// the only form of callback
typedef void (*pullfn)(void *info);

// initialize the helper library - call this before anything else
// note: memory leaks can occur if ii_init is called more than once
void ii_init(void);

// call just before a clean exit to cleanly close all modules
void ii_death(void);

// adds a callback to be called for every sample
void ii_sampler_call(pullfn sampler, void *info);

// adds a callback to be called every frequency samples
void ii_control_call(pullfn control, void *info, int frequency);

// runs *dst = *src for every sample
void ii_sampler_move(float *src, float *dst);

// when the system is closed, these functions are run
void ii_death_call(pullfn fn, void *info);

// run for a certain number of samples
void ii_run(int samples);

#endif
