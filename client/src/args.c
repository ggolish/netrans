
#include "args.h"
#include "error.h"

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ARG_DESCRIPTION 100
#define NUM_ARGS 4

static char arguments[NUM_ARGS][2][MAX_ARG_DESCRIPTION] = {
    {"-h", "Print out usage information"},
    {"-l", "Enable loopback mode, transfer occurs on same machine [required without target-machine]"},
    {"-d <network-device>", "The name of the network device to use, default is 'eth0'"},
    {"target-machine", "The machine to transfer to, can be n1, n2, or n3 [required without -l]"}
};

static netrans_args_t *args_init();
static void usage(char *name);
static int parse_target_machine(netrans_args_t *args, char *arg);

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

    if((optind >= argc) && (!args->loopback)) {
        sprintf(err_msg, "Must specify either '-l' or target-machine");
        return NULL;
    } else if((optind < argc) && (args->loopback)) {
        sprintf(err_msg, "Cannot specify both loopback and target-machine");
        return NULL;
    } else if(optind < argc) {
        if(parse_target_machine(args, argv[optind]) == -1) {
            sprintf(err_msg, "Invalid target machine '%s'", argv[optind]);
            return NULL;
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
    args->target_machine = -1;
    return args;
}

static void usage(char *name)
{
    fprintf(stderr, "Usage: %s [-d <network device>] <-l | target-machine>\n", name);
    for(int i = 0; i < NUM_ARGS; ++i) {
        fprintf(stderr, "  %-20s %s\n", arguments[i][0], arguments[i][1]);
    }
}

static int parse_target_machine(netrans_args_t *args, char *arg)
{
    if(strcmp(arg, "n1") == 0 || strcmp(arg, "n2") == 0 || strcmp(arg, "n3") == 0) {
        args->target_machine = atoi(&arg[1]) - 1;
        return 1;
    }
    return -1;
}
