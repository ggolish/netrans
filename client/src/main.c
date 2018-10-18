#include "netrans.h"
#include "error.h"
#include "args.h"

#include <stdlib.h>

int main(int argc, char *argv[])
{
    int sockfd;
    netrans_args_t *args;

    args = args_process(argc, argv);

    if(!args) {
        die(EXIT_FAILURE);
    }

    #ifdef NETRANS_DEBUG
        args_print(args);
    #endif

    if((sockfd = netrans_init(args->net_device, args->loopback)) == -1) {
        die(EXIT_FAILURE);
    }

    if(args->send && netrans_send(sockfd, args->target_machine, args->local_path, args->remote_path) == -1) {
        die(EXIT_FAILURE);
    }

    if(args->receive && netrans_receive(sockfd, args->target_machine, args->local_path, args->remote_path) == -1) {
        die(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
