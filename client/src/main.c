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

    if((sockfd = netrans_init(args->net_device, args->loopback)) == -1) {
        die(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
