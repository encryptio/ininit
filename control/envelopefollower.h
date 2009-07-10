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

#ifndef _CONTROL_ENVELOPEFOLLOWER_H_
#define _CONTROL_ENVELOPEFOLLOWER_H_

struct control_envelopefollower_st {
    float now;
    float drop;
    float *input;
};

// !lua:control_envelopefollower -> control_envelopefollower_make(float *input, float drop)
/* !doc:control_envelopefollower(input, dropamt)
 *      Returns a signal representing the volume envelope represented by the
 *      given signal. The dropamt parameter represents the amount of decay in
 *      the envelope created. A larger dropamt is more responsive, but also
 *      introduces more noise into the signal.
 */
float * control_envelopefollower_make(float *input, float drop);

#endif

