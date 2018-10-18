#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void die(int exit_code)
{
    if(strlen(err_msg) > 0)
        fprintf(stderr, "Error: %s\n", err_msg);
    exit(exit_code);
}

void noerror()
{
    err_msg[0] = '\0';
}
