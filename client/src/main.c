#include "netrans.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int sockfd;

    if(argc != 2) {
        usage(argv);
    }

    if((sockfd = netrans_init(argv[1])) == -1) {
        die(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
