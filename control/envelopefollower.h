// Copyright 2009 Jack Christopher Kastorff

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

