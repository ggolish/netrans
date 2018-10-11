#include "error.h"

#include <stdio.h>
#include <stdlib.h>

void die(int exit_code)
{
    fprintf(stderr, "Error: %s\n", err_msg);
    exit(exit_code);
}
