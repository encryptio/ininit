// Copyright 2009 Jack Christopher Kastorff

#include <stdlib.h>
#include <unistd.h>

#include "output/openal.h"
#include "die.h"
#include "helpers.h"

void output_openal_ticker(void * info) {
    struct output_openal_st *me = (struct output_openal_st *)info;
    ALint val;

    me->buf[me->bufat++] = (ALCshort) ( 32768 * *me->input );

    if ( me->bufat >= OUTPUT_OPENAL_BUFFER_SIZE/sizeof(ALCshort) ) {
        if ( me->albuffersused < OUTPUT_OPENAL_NUM_BUFFERS ) {
            alBufferData(me->buffers[me->whichalbuf], AL_FORMAT_MONO16, me->buf, OUTPUT_OPENAL_BUFFER_SIZE, (ALuint) *sample_rate);
            alSourceQueueBuffers(me->source, 1, &me->buffers[me->whichalbuf]);
            me->whichalbuf++;
            me->albuffersused++;
        } else {
            alGetSourcei(me->source, AL_SOURCE_STATE, &val);
            if ( val != AL_PLAYING ) {
                alSourcePlay(me->source);
                if ( alGetError() == AL_INVALID_OPERATION )
                    die("ctx");
            }

            while ( 1 ) {
                alGetSourcei(me->source, AL_BUFFERS_PROCESSED, &val);
                if ( val > 0 ) break;
                usleep(5000); // 5ms
            }

            alSourceUnqueueBuffers(me->source, 1, me->buffers);
            alBufferData(me->buffers[me->whichalbuf], AL_FORMAT_MONO16, me->buf, OUTPUT_OPENAL_BUFFER_SIZE, (ALuint) *sample_rate);
            alSourceQueueBuffers(me->source, 1, &me->buffers[me->whichalbuf]);
            me->whichalbuf++;

            alGetSourcei(me->source, AL_SOURCE_STATE, &val);
            if ( val != AL_PLAYING ) {
                alSourcePlay(me->source);
                if ( alGetError() == AL_INVALID_OPERATION )
                    die("ctx");
            }
        }

        me->bufat = 0;

        if ( me->whichalbuf >= OUTPUT_OPENAL_NUM_BUFFERS )
            me->whichalbuf = 0;
    }
}

void output_openal_make(float *input) {
    struct output_openal_st *ret;

    if ( (ret = malloc(sizeof(*ret))) == NULL )
        die("output_openal_make: couldn't malloc ret");

    if ( (ret->dev = alcOpenDevice(NULL)) == NULL )
        die("output_openal_make: alcOpenDevice failed");

    if ( (ret->ctx = alcCreateContext(ret->dev,NULL)) == NULL )
        die("output_openal_make: alcCreateContext failed");

    alcMakeContextCurrent(ret->ctx);

    alGenBuffers(OUTPUT_OPENAL_NUM_BUFFERS, ret->buffers);
    alGenSources(1, &ret->source);
    if ( alGetError() != AL_NO_ERROR )
        die("output_openal_make: couldn't generate things");
    ret->albuffersused = 0;
    ret->input = input;

    if ( (ret->buf = malloc(OUTPUT_OPENAL_BUFFER_SIZE)) == NULL )
        die("output_openal_make: couldn't malloc space for ret->buf");
    ret->bufat = 0;

    // and set up the sampler
    ii_sampler_call(output_openal_ticker, (void *)ret);
}

