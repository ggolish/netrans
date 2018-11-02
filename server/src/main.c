#include "common.h"
#include "server.h"
#include "args.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    netrans_args_t *args;
    int sockfd;

    args = args_process(argc, argv);

    if(!args) {
        die(EXIT_FAILURE);
    }

    #ifdef NETRANS_DEBUG
        args_print(args);
    #endif

    if((sockfd = netrans_init(args->net_device, 0)) == -1) {
        die(EXIT_FAILURE);
    }

    if(netransd_mainloop(sockfd) == -1) {
        die(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
