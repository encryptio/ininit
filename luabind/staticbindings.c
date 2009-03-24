// Copyright 2009 Jack Christopher Kastorff

#include <stdlib.h>

#include "luabind/bind.h"

#include "saver.h"
#include "die.h"
#include "helpers.h"

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

// !lua:saver -> bind_saver
static int bind_saver(lua_State *lst) {
    float *input;
    char *path;

    if ( (input = lua_touserdata(lst, 1)) == NULL )
        die("bind_saver: first argument is not a signal");

    path = (char *)luaL_checkstring(lst, 2);

    saver_make(input, path);
    return 0;
}

// !lua:getsamplerate -> bind_getsamplerate
static int bind_getsamplerate(lua_State *lst) {
    lua_pushnumber(lst, sample_rate);
    return 1;
}

// !lua:runsamples -> bind_runsamples
static int bind_runsamples(lua_State *lst) {
    ii_run( luaL_checkint(lst, 1) );
    return 0;
}

// !lua:run -> bind_run
static int bind_run(lua_State *lst) {
    ii_run( (int)( (float)luaL_checknumber(lst, 1) * sample_rate ) );
    return 0;
}

struct lua_boundfn_st {
    float now;
    int refnum;
    lua_State *lst;
    float **inputs;
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
    me->now = (float) luaL_checknumber(me->lst, -1);
}

// !lua:makefn -> bind_makefn
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
    if ( (me->inputs = malloc(sizeof(float*) * me->inputcount)) == NULL )
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

