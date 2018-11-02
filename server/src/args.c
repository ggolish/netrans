
#include "args.h"
#include "error.h"

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ARG_DESCRIPTION 100
#define NUM_ARGS 2
#define DEFAULT_NET_DEVICE "eth0"

static char arguments[NUM_ARGS][2][MAX_ARG_DESCRIPTION] = {
    {"-h", "Print out usage information"},
    {"-d <network-device>", "The name of the network device to use, default is 'eth0'."}
};

static netrans_args_t *args_init();
static void usage(char *name);
static void help();

netrans_args_t *args_process(int argc, char *argv[])
{
    netrans_args_t *args = args_init();
    int opt;

    while((opt = getopt(argc, argv, "d:h")) != -1) {
        switch(opt) {
            case 'd':
                args->net_device = strdup(optarg);
                break;
            case 'h':
                usage(argv[0]);
                help();
                exit(EXIT_SUCCESS);
            default:
                break;
        }
    }

    return args;
}

void args_print(netrans_args_t *args)
{
    printf("\n--ARGS-------------\n");
    printf("Network Device: %s\n", args->net_device);
    printf("-------------------\n\n");
}

static netrans_args_t *args_init()
{
    netrans_args_t *args;

    args = (netrans_args_t *)malloc(sizeof(netrans_args_t));
    args->net_device = DEFAULT_NET_DEVICE;
    return args;
}

static void usage(char *name)
{
    fprintf(stderr, "Usage: %s [-d <network device>]\n", name);
}

static void help()
{
    for(int i = 0; i < NUM_ARGS; ++i) {
        fprintf(stderr, "  %-20s %s\n", arguments[i][0], arguments[i][1]);
    }
}
