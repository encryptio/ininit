// Copyright 2009 Jack Christopher Kastorff

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "helpers.h"
#include "luabind/bind.h"
#include "die.h"

lua_State *lst;

// use standard malloc procedures
void * alloclua(void *ud, void *ptr, size_t osize, size_t nsize) {
    void * ret;
    if ( nsize == 0 ) {
        free(ptr);
        return NULL;
    } else {
        return realloc(ptr, nsize);
    }
}

void interpereter_loop(void) {
    char line[1000];

    while ( gets(line) != 0 ) {
        if ( luaL_dostring(lst, line) ) {
            printf("error: %s\n", luaL_checkstring(lst, lua_gettop(lst)));
        }
    }
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

    if ( argc == 1 ) {
        printf("Lua interpereter loop running.\n");
        interpereter_loop();
    } else {
        if ( luaL_dofile(lst, argv[1]) ) {
            printf("error while executing %s: %s\n", argv[1], luaL_checkstring(lst, lua_gettop(lst)));
            exit(1);
        }
    }

    ii_death();

    exit(0);
}

