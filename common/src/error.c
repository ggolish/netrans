#include "error.h"

#include <stdio.h>
#include <stdlib.h>

void die(int exit_code)
{
    fprintf(stderr, "Error: %s\n", err_msg);
    exit(exit_code);
}

void usage(char *argv[])
{
    fprintf(stderr, "Usage: %s <network-device>\n", argv[0]);
    exit(EXIT_FAILURE);
}
