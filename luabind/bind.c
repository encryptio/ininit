// Copyright 2009 Jack Christopher Kastorff

#include <stdlib.h>

#include "luabind/bind.h"

#include "saver.h"
#include "die.h"
#include "osc/sawtooth.h"
#include "helpers.h"

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

static int bind_osc_sawtooth(lua_State *lst) {
    double phase;
    double *frequency;
    void *ret;

    phase = (double) luaL_checknumber(lst, 1);

    if ( (frequency = (double *) lua_touserdata(lst, 2)) == NULL ) {
        if ( (frequency = malloc(sizeof(double))) == NULL )
            die("bind_osc_sawtooth: couldn't malloc frequency");

        *frequency = (double) luaL_checknumber(lst, 2);
    }

    ret = (void *) osc_sawtooth_make(phase, frequency);

    lua_pushlightuserdata(lst, ret);
    return 1;
}


// :saver -> bind_saver
static int bind_saver(lua_State *lst) {
    double *input;
    char *path;

    if ( (input = lua_touserdata(lst, 1)) == NULL )
        die("bind_saver: first argument is not a signal");

    path = (char *)luaL_checkstring(lst, 2);

    saver_make(input, path);
    return 0;
}

// :getsamplerate -> bind_getsamplerate
static int bind_getsamplerate(lua_State *lst) {
    lua_pushnumber(lst, sample_rate);
    return 1;
}

// :runsamples -> bind_runsamples
static int bind_runsamples(lua_State *lst) {
    ii_run( luaL_checkint(lst, 1) );
    return 0;
}

// :run -> bind_run
static int bind_run(lua_State *lst) {
    ii_run( (int)( (double)luaL_checknumber(lst, 1) * sample_rate ) );
    return 0;
}

struct lua_boundfn_st {
    double now;
    int refnum;
    lua_State *lst;
    double **inputs;
    int inputcount;
};

void bind_makefn_ticker(void * info) {
    struct lua_boundfn_st * me = (struct lua_boundfn_st *) info;
    int i;

    // push the function
    lua_rawgeti(me->lst, LUA_REGISTRYINDEX, me->refnum);

    // and the arguments
    for (i=0; i<me->inputcount; i++)
        lua_pushnumber(me->lst, (lua_Number) *(me->inputs[i]));

    // call it
    lua_call(me->lst, me->inputcount, 1);

    // and grab the result
    me->now = (double) luaL_checknumber(me->lst, -1);
}

// :makefn -> bind_makefn
static int bind_makefn(lua_State *lst) {
    int i;
    int argc = lua_gettop(lst);
    struct lua_boundfn_st * me;

    int frequency = luaL_checkint(lst, 1);
    luaL_checktype(lst, 2, LUA_TFUNCTION);

    for (i=3; i<argc; i++)
        luaL_checktype(lst, i, LUA_TLIGHTUSERDATA);

    if ( frequency < 1 )
        frequency = 1;

    if ( (me = malloc(sizeof(*me))) == NULL )
        die("bind_makefn: couldn't malloc me");

    lua_pushvalue(lst, 2);
    me->refnum = luaL_ref(lst, LUA_REGISTRYINDEX);
    me->now = 0;
    me->lst = lst;
    me->inputcount = argc - 2;
    if ( (me->inputs = malloc(sizeof(double*) * me->inputcount)) == NULL )
        die("bind_makefn: couldn't malloc me->inputs");

    for (i=0; i<me->inputcount; i++) {
        if ( (me->inputs[i] = lua_touserdata(lst, i+3)) == NULL )
            die("bind_makefn: argument is not a signal");
    }

    if ( frequency == 1 ) {
        ii_sampler_call(bind_makefn_ticker, (void *)me);
    } else {
        ii_control_call(bind_makefn_ticker, (void *)me, frequency);
    }

    lua_pushlightuserdata(lst, me);
    return 1;
}

void bind_ininit_lua_fns(lua_State *lst) {
    lua_register(lst, "sawtooth", bind_osc_sawtooth);
    lua_register(lst, "saver", bind_saver);
    lua_register(lst, "getsamplerate", bind_getsamplerate);
    lua_register(lst, "runsamples", bind_runsamples);
    lua_register(lst, "run", bind_run);
    lua_register(lst, "makefn", bind_makefn);
}
