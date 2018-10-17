
#include "args.h"

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ARG_DESCRIPTION 100
#define NUM_ARGS 3

static char arguments[NUM_ARGS][2][MAX_ARG_DESCRIPTION] = {
    {"-h", "Print out usage information"},
    {"-l", "Enable loopback mode, transfer occurs on same machine"},
    {"-d <network-device>", "The name of the network device to use, default is 'eth0'"}
};

static netrans_args_t *args_init();
static void usage(char *name);

netrans_args_t *args_process(int argc, char *argv[])
{
    netrans_args_t *args = args_init();
    int opt;

    while((opt = getopt(argc, argv, "ld:h")) != -1) {
        switch(opt) {
            case 'd':
                args->net_device = strdup(optarg);
                break;
            case 'l':
                args->loopback = 1;
                break;
            case 'h':
                usage(argv[0]);
                exit(EXIT_SUCCESS);
            default:
                break;
        }
    }

    return args;
}

static netrans_args_t *args_init()
{
    netrans_args_t *args;

    args = (netrans_args_t *)malloc(sizeof(netrans_args_t));
    args->net_device = NULL;
    args->loopback = 0;
    return args;
}

static void usage(char *name)
{
    fprintf(stderr, "Usage: %s [-d <network device>] [-l]\n", name);
    for(int i = 0; i < NUM_ARGS; ++i) {
        fprintf(stderr, "  %-20s %s\n", arguments[i][0], arguments[i][1]);
    }
}
