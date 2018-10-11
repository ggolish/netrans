#include "netransd.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int sockfd;

    if((sockfd = netransd_init()) == -1) {
        die(EXIT_FAILURE);
    }

    if(netransd_mainloop(sockfd) == -1) {
        die(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
