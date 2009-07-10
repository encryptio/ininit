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

#ifndef _CONTROL_EXPANDTRIGGER_H_
#define _CONTROL_EXPANDTRIGGER_H_

struct control_expandtrigger_st {
    float now;
    int lastmark;
    int triggered;
    float *length;
    float *input;
};

// !lua:control_expandtrigger -> control_expandtrigger_make(float *input, float *length)
/* !doc:control_expandtrigger(input, length)
 *      Takes the given input trigger and expands its pulses (forward in time)
 *      to length seconds. Often useful in conjunction with control_totrigger.
 */
float * control_expandtrigger_make(float *input, float *length);

#endif

