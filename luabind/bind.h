// Copyright 2009 Jack Christopher Kastorff

#ifndef _LUABIND_BIND_H_
#define _LUABIND_BIND_H_

#include "lua.h"

void bind_ininit_lua_fns(lua_State * st);
void lua_set_arguments(int argc, char **argv);

#endif

