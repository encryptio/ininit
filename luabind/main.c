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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "helpers.h"
#include "luabind/bind.h"
#include "die.h"

// experimentally derived, may not be optimal for all programs

// in samples
#define GARBAGE_COLLECT_INTERVAL 2000

#define GARBAGE_STEPMULT 180
#define GARBAGE_PAUSE    150
#define GARBAGE_INCAMOUNT 2

#define INTERPERETER_LINE_LENGTH 1000

lua_State *lst;

// use standard malloc procedures
void * alloclua(void *ud, void *ptr, size_t osize, size_t nsize) {
    if ( nsize == 0 ) {
        free(ptr);
        return NULL;
    } else {
        return realloc(ptr, nsize);
    }
}

void interpereter_loop(void) {
    char line[INTERPERETER_LINE_LENGTH];

    while ( fgets(line,INTERPERETER_LINE_LENGTH,stdin) != 0 ) {
        if ( luaL_dostring(lst, line) ) {
            fprintf(stderr, "error: %s\n", luaL_checkstring(lst, lua_gettop(lst)));
        }
    }
}

void dosomegarbagecollection(void *info) {
    lua_gc(lst, LUA_GCSTEP, GARBAGE_INCAMOUNT);
}

void initialize_lua(void) {
    lst = lua_newstate(alloclua, NULL);

    // core lua libraries
    luaL_openlibs(lst);

    // ininit library
    bind_ininit_lua_fns(lst);
}

int main (int argc, char **argv) {
    ii_init();
    srand((unsigned) time(NULL));
    initialize_lua();

    lua_set_arguments(argc, argv);

    lua_gc(lst, LUA_GCSETSTEPMUL, GARBAGE_STEPMULT);
    lua_gc(lst, LUA_GCSETPAUSE,   GARBAGE_PAUSE);

    ii_control_call(dosomegarbagecollection, NULL, GARBAGE_COLLECT_INTERVAL);

    if ( argc == 1 ) {
        if ( isatty(fileno(stdout)) )
            printf("Lua interpereter loop running.\n");
        interpereter_loop();
    } else {
        if ( luaL_dofile(lst, argv[1]) ) {
            fprintf(stderr, "error while executing %s: %s\n", argv[1], luaL_checkstring(lst, lua_gettop(lst)));
            exit(1);
        }
    }

    ii_death();

    exit(0);
}

