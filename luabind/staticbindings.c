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
/* !doc:saver(signal, [signal, ...], filename)
 *      Saves the signals as channels in an .au file named by the last argument.
 */
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
/* !doc:getcurrentsample()
 *      Returns the current sample position in the render.
 */
static int bind_getcurrentsample(lua_State *lst) {
    lua_pushinteger(lst, *current_sample);
    return 1;
}

// !lua:getsamplerate -> bind_getsamplerate
/* !doc:getsamplerate()
 *      Returns the sample rate.
 */
static int bind_getsamplerate(lua_State *lst) {
    lua_pushnumber(lst, *sample_rate);
    return 1;
}

// !lua:gettime -> bind_gettime
/* !doc:gettime()
 *      Returns the current time in seconds. Equivalent to
 *      getcurrentsample()/getsamplerate().
 */
static int bind_gettime(lua_State *lst) {
    lua_pushnumber(lst, ((double) *current_sample) / ((double) *sample_rate));
    return 1;
}

// !lua:runsamples -> bind_runsamples
/* !doc:runsamples(count)
 *      Runs the currently defined systems for count samples. See also "run".
 */
static int bind_runsamples(lua_State *lst) {
    ii_run( luaL_checkint(lst, 1) );
    return 0;
}

// !lua:run -> bind_run
/* !doc:run(seconds)
 *      Runs the currently defined systems for the given amount of time.
 */
static int bind_run(lua_State *lst) {
    ii_run( (int)( (float)luaL_checknumber(lst, 1) * *sample_rate ) );
    return 0;
}

struct lua_cycletable_st {
    float now;
    int at;
    float **cycle;
    int cyclecount;
    int timelength;
    int currentpoint;
};

void bind_cycletable_ticker(void * info) {
    struct lua_cycletable_st * me = (struct lua_cycletable_st *) info;

    me->currentpoint++;
    if ( me->currentpoint > me->timelength ) {
        me->currentpoint -= me->timelength;

        me->at++;
        if ( me->at >= me->cyclecount )
            me->at = 0;
    }

    me->now = *(me->cycle[me->at]);
}

// !lua:cycletable -> bind_cycletable
/* !doc:cycletable(length, table)
 *      Takes a table of signals and a length in seconds, returns a new signal
 *      that represents switching to each signal in the table in sequence every
 *      length seconds.
 *      
 *      Examples:
 *
 *      mark = { 1, 0, 1, 0, 1, 1, 1, 0 }
 *      trigger = control_totrigger( cycletable(0.095, mark) )
 *
 *      frtable = { 110, 220, 110, 110, 55, 110, 220, 110 }
 *      freq = cycletable(0.3, frtable)
 */
static int bind_cycletable(lua_State *lst) {
    int argc = lua_gettop(lst);
    struct lua_cycletable_st * me;
    int inputcount, i;

    if ( (me = malloc(sizeof(*me))) == NULL )
        die("bind_cycletable: couldn't malloc me");

    me->timelength = (int) ((double) luaL_checknumber(lst, 1) * *sample_rate);
    me->at = 0;
    me->now = 0;
    me->currentpoint = 0;
    me->cyclecount = lua_objlen(lst, 2);

    if ( (me->cycle = malloc(sizeof(float*) * me->cyclecount)) == NULL )
        die("bind_cycletable: couldn't malloc me->cycle");

    // iterate over the items of the table
    i = 0;
    lua_pushnil(lst);
    while ( lua_next(lst, 2) ) {
        if ( (me->cycle[i] = lua_touserdata(lst, -1)) == NULL ) {
            if ( (me->cycle[i] = malloc(sizeof(float*))) == NULL )
                die("bind_cycletable: couldn't malloc space for constant signal");

            *(me->cycle[i]) = (double) luaL_checknumber(lst, -1);
        }

        lua_pop(lst, 1); // take out the value, leaving the last key
        i++;
    }

    ii_sampler_call(bind_cycletable_ticker, (void *)me);

    lua_pushlightuserdata(lst, &me->now);
    return 1;
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
/* !doc:makefn(frequency, function, [signal, ...])
 *      Creates a new signal representing the return value of the given
 *      function when given arguments of the current values of the given
 *      signals, run every frequency samples.
 *
 *      Example:
 *
 *      fr = makefn(50, function (x) return x*10+440 end, osc_sine(0, 0.7))
 *      wave = osc_sine(0, fr)
 */
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



struct lua_signals_table_fold_st {
    float now;
    float **inputs;
    int inputcount;
    float multiply;
    int multiplyall;
};

void bind_signals_table_fold_ticker(void * info) {
    struct lua_signals_table_fold_st * me = (struct lua_signals_table_fold_st *) info;
    float new;
    int i;

    if ( me->multiplyall ) {
        new = 1;
        for (i=0; i<me->inputcount; i++) {
            new *= *(me->inputs[i]);
        }
    } else {
        new = 0;
        for (i=0; i<me->inputcount; i++) {
            new += *(me->inputs[i]);
        }
    }

    me->now = new * me->multiply;
}

static int bind_signals_table_fold(lua_State *lst, int fromaverage, int domultiply) {
    struct lua_signals_table_fold_st * me;
    int tablecount, i;

    if ( (me = malloc(sizeof(*me))) == NULL )
        die("bind_signals_table_fold: couldn't malloc me");

    luaL_checktype(lst, 1, LUA_TTABLE);

    if ( (me->inputcount = lua_objlen(lst, 1)) == 0 )
        die("bind_signals_table_fold: table is empty");

    if ( (me->inputs = malloc(sizeof(float*) * me->inputcount)) == NULL )
        die("bind_signals_table_fold: couldn't malloc inputs");

    // iterate over the items of the table
    i = 0;
    lua_pushnil(lst);
    while ( lua_next(lst, 1) ) {
        if ( (me->inputs[i] = lua_touserdata(lst, -1)) == NULL ) {
            if ( (me->inputs[i] = malloc(sizeof(float*))) == NULL )
                die("bind_signals_table_fold: couldn't malloc space for constant signal");

            *(me->inputs[i]) = luaL_checknumber(lst, -1);
        }

        lua_pop(lst, 1); // take out the value, leaving the last key
        i++;
    }

    me->now = 0;
    me->multiply = fromaverage ? 1.0/me->inputcount : 1;
    me->multiplyall = domultiply;
    ii_sampler_call(bind_signals_table_fold_ticker, (void *)me);

    lua_pushlightuserdata(lst, me);
    return 1;
}

// !lua:signals_table_add -> bind_signals_table_add
/* !doc:signals_table_add(table)
 *      Returns a signal representing the sum of the values of all the signals
 *      in the given table.
 */
static int bind_signals_table_add(lua_State *lst) {
    return bind_signals_table_fold(lst, 0, 0);
}

// !lua:signals_table_average -> bind_signals_table_average
/* !doc:signals_table_average(table)
 *      Returns a signal representing the average of the values of all the
 *      signals in the given table.
 */
static int bind_signals_table_average(lua_State *lst) {
    return bind_signals_table_fold(lst, 1, 0);
}

// !lua:signals_table_multiply -> bind_signals_table_multiply
/* !doc:signals_table_multiply(table)
 *      Returns a signal representing the product of all the values of all the
 *      signals in the given table.
 */
static int bind_signals_table_multiply(lua_State *lst) {
    return bind_signals_table_fold(lst, 0, 1);
}

static int lua_arguments_argc;
static char **lua_arguments_argv;

void lua_set_arguments(int argc, char **argv) {
    lua_arguments_argc = argc;
    lua_arguments_argv = argv;
}

// !lua:getargs -> bind_get_arguments
/* !doc:getargs()
 *      Returns a list of strings that are the program arguments.
 *
 *      Example:
 *
 *      -- run with: ininit script.lua outfile.au
 *      filename = getargs()
 *      saver(wave, filename)
 */
static int bind_get_arguments(lua_State *lst) {
    int i;
    for (i=2; i<lua_arguments_argc; i++)
        lua_pushstring(lst, lua_arguments_argv[i]);
    return lua_arguments_argc-2;
}

