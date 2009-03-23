// Copyright 2009 Jack Christopher Kastorff

#include <stdio.h>
#include <stdlib.h>

#include "die.h"

void die(const char * msg) {
    fprintf(stderr, "Died: %s\n", msg);
    exit(EXIT_FAILURE);
}

