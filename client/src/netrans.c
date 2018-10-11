#include "netrans.h"
#include "error.h"
#include "packet.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <stdio.h>
#include <string.h>

int netrans_init()
{
    int sockfd;

    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if(sockfd == -1) {
        sprintf(err_msg, "Unable to open raw socket");
        return -1;
    }

    return sockfd;
}
