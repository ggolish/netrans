#include "common.h"
#include "server.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int sockfd;

    if((sockfd = netrans_init(NULL, 0)) == -1) {
        die(EXIT_FAILURE);
    }

    if(netransd_mainloop(sockfd) == -1) {
        die(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
