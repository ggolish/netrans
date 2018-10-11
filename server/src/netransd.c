#include "netransd.h"
#include "packet.h"
#include "error.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <stdio.h>
#include <string.h>

#define K 1024

static int process_packet(char *buffer);

int netransd_init()
{
    int sockfd;

    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if(sockfd == -1) {
        sprintf(err_msg, "Unable to open raw socket");
        return -1;
    }

    return sockfd;
}

int netransd_mainloop(int sockfd)
{
    char buffer[4 * K];
    unsigned int addrlen;
    struct sockaddr_ll from;

    for(;;) {
        int len = recvfrom(sockfd, buffer, 4 * K, MSG_DONTWAIT, (struct sockaddr *)(&from), &addrlen);
        if(len > 0) {
            int rv = process_packet(buffer);
        }
    }
}

static int process_packet(char *buffer)
{
    PACKET_ETH_HDR eth_hdr;

    memset(&eth_hdr, 0, sizeof(PACKET_ETH_HDR));
    memcpy(&eth_hdr, buffer, sizeof(PACKET_ETH_HDR));
    printf("%04x\n", ntohs(eth_hdr.eth_type));
    return 0;
}
