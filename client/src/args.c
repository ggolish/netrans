
#include "common.h"
#include "args.h"

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ARG_DESCRIPTION 100
#define NUM_ARGS 9

static char arguments[NUM_ARGS][2][MAX_ARG_DESCRIPTION] = {
    {"-h", "Print out usage information"},
    {"-l", "Enable loopback mode, transfer occurs on same machine [required without target-machine]."},
    {"-d <network-device>", "The name of the network device to use, default is 'eth0'."},
    {"-r", "Receive file <path> from <target-machine> [required without -s]."},
    {"-s", "Send file <path> to <target-machine> [required without -r]."},
    {"-v", "Enable verbosity."},
    {"target-machine", "The machine to transfer to, can be n1, n2, or n3 [required without -l]."},
    {"local-path", "The path to file on local machine."},
    {"remote-path", "The path to file on remote machine."}
};

static netrans_args_t *args_init();
static void usage(char *name);
static void help();
static int parse_target_machine(netrans_args_t *args, char *arg);

netrans_args_t *args_process(int argc, char *argv[])
{
    netrans_args_t *args = args_init();
    int opt, diff;

    while((opt = getopt(argc, argv, "ld:hsrv")) != -1) {
        switch(opt) {
            case 'd':
                args->net_device = strdup(optarg);
                break;
            case 'l':
                args->loopback = 1;
                break;
            case 's':
                args->send = 1;
                break;
            case 'r':
                args->receive = 1;
                break;
            case 'v':
                args->verbose = 1;
                break;
            case 'h':
                usage(argv[0]);
                help();
                exit(EXIT_SUCCESS);
            default:
                break;
        }
    }

    if(!args->receive && !args->send) {
        usage(argv[0]);
        sprintf(err_msg, "Must specify either '-s' or '-r'");
        return NULL;
    }

    if(args->receive && args->send) {
        usage(argv[0]);
        sprintf(err_msg, "Cannot specify both '-r' and '-s'");
        return NULL;
    }

    diff = argc - optind;

    if(diff != 3) {
        if(diff == 2 && args->loopback) {
            args->local_path = strdup(argv[optind]);
            args->remote_path = strdup(argv[optind + 1]);
        } else {
            usage(argv[0]);
            noerror();
            return NULL;
        }
    } else {

        if(parse_target_machine(args, argv[optind]) == -1) {
            sprintf(err_msg, "Invalid target machine '%s'", argv[optind]);
            return NULL;
        }

        args->local_path = strdup(argv[optind + 1]);
        args->remote_path = strdup(argv[optind + 2]);
    }

    return args;
}

void args_print(netrans_args_t *args)
{
    printf("\n--ARGS-------------\n");
    printf("Network Device: %s\n", args->net_device);
    if(args->send) printf("Sending\n");
    if(args->receive) printf("Receiving\n");
    if(args->loopback) printf("Loopback\n");
    else printf("Target Machine: n%d\n", args->target_machine + 1);
    printf("Local Path: %s\n", args->local_path);
    printf("Remote Path: %s\n", args->remote_path);
    printf("-------------------\n\n");
}

static netrans_args_t *args_init()
{
    netrans_args_t *args;

    args = (netrans_args_t *)malloc(sizeof(netrans_args_t));
    args->net_device = DEFAULT_NET_DEVICE;
    args->send = args->receive = 0;
    args->verbose = 0;
    args->loopback = 0;
    args->target_machine = NETRANS_LOOPBACK_ID;
    return args;
}

static void usage(char *name)
{
    fprintf(stderr, "Usage: %s [-d <network device>] [-v] <-s | -r> <-l | target-machine> <local-path> <remote-path>\n", name);
}

static void help()
{
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
    return NETRANS_LOOPBACK_ID;
}
