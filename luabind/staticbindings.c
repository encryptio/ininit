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
    int argc = lua_gettop(lst);
    float **inputs;
    char *path;
    int num_inputs, i;

    num_inputs = argc-1;

    if ( (inputs = malloc(sizeof(float *) * num_inputs)) == NULL )
        die("bind_saver: couldn't malloc inputs");

    for (i=1; i<=num_inputs; i++) {
        if ( (inputs[i-1] = lua_touserdata(lst, i)) == NULL )
            die("bind_saver: nonlast argument is not a signal");
    }

    path = (char *)luaL_checkstring(lst, -1);

    saver_make(inputs, num_inputs, path);
    return 0;
}

// !lua:getcurrentsample -> bind_getcurrentsample
static int bind_getcurrentsample(lua_State *lst) {
    lua_pushinteger(lst, *current_sample);
    return 1;
}

// !lua:getsamplerate -> bind_getsamplerate
static int bind_getsamplerate(lua_State *lst) {
    lua_pushnumber(lst, *sample_rate);
    return 1;
}

// !lua:runsamples -> bind_runsamples
static int bind_runsamples(lua_State *lst) {
    ii_run( luaL_checkint(lst, 1) );
    return 0;
}

// !lua:run -> bind_run
static int bind_run(lua_State *lst) {
    ii_run( (int)( (float)luaL_checknumber(lst, 1) * *sample_rate ) );
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



struct lua_signals_table_add_st {
    float now;
    float **inputs;
    int inputcount;
    float multiply;
};

void bind_signals_table_add_ticker(void * info) {
    struct lua_signals_table_add_st * me = (struct lua_signals_table_add_st *) info;
    float new = 0;
    int i;

    for (i=0; i<me->inputcount; i++) {
        new += *(me->inputs[i]);
    }

    me->now = new * me->multiply;
}

static int bind_signals_table_sum(lua_State *lst, int fromaverage) {
    struct lua_signals_table_add_st * me;
    int tablecount, i;

    if ( (me = malloc(sizeof(*me))) == NULL )
        die("bind_signals_table_add: couldn't malloc me");

    luaL_checktype(lst, 1, LUA_TTABLE);

    if ( (me->inputcount = lua_objlen(lst, 1)) == 0 )
        die("bind_signals_table_add: table is empty");

    if ( (me->inputs = malloc(sizeof(float*) * me->inputcount)) == NULL )
        die("bind_signals_table_add: couldn't malloc inputs");

    // iterate over the items of the table
    i = 0;
    lua_pushnil(lst);
    while ( lua_next(lst, 1) ) {
        if ( (me->inputs[i++] = lua_touserdata(lst, -1)) == NULL )
            die("bind_signals_table_add: not all values in the table are signals");

        lua_pop(lst, 1); // take out the value, leaving the last key
    }

    me->now = 0;
    me->multiply = fromaverage ? 1/me->inputcount : 1;
    ii_sampler_call(bind_signals_table_add_ticker, (void *)me);

    lua_pushlightuserdata(lst, me);
    return 1;
}

// !lua:signals_table_add -> bind_signals_table_add
static int bind_signals_table_add(lua_State *lst) {
    return bind_signals_table_sum(lst, 0);
}

// !lua:signals_table_average -> bind_signals_table_average
static int bind_signals_table_average(lua_State *lst) {
    return bind_signals_table_sum(lst, 1);
}

