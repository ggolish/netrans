#include "netrans.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int sockfd, opt;
    int loopback = 0;
    char *net_device = NULL;

    while((opt = getopt(argc, argv, "ld:")) != -1) {
        switch(opt) {
            case 'd':
                net_device = strdup(optarg);
                break;
            case 'l':
                loopback = 1;
                break;
            default:
                break;
        }
    }

    if((sockfd = netrans_init(net_device, loopback)) == -1) {
        die(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
