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

#ifndef _CONTROL_BROWNIAN_H_
#define _CONTROL_BROWNIAN_H_

struct control_brownian_st {
    float fnow;
    float *filterat;
    double now;
    double hp_last_out;
    double hp_last_in;
};

// !lua:control_brownian -> control_brownian_make(float *filterat)
/* !doc:control_brownian(filterat)
 *      TODO
 */
float * control_brownian_make(float *filterat);

#endif

