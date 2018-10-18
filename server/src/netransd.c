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
            process_packet(buffer);
        }
    }
}

static int process_packet(char *buffer)
{
    PACKET_ETH_HDR *eth_hdr;
    PACKET_NETRANS_HDR *netrans_hdr;
    uint16_t type;

    eth_hdr = (PACKET_ETH_HDR *)buffer;
    type = ntohs(eth_hdr->eth_type);
    if(type != ETH_TYPE_NETRANS) return 0;
    netrans_hdr = (PACKET_NETRANS_HDR *)(buffer + sizeof(PACKET_ETH_HDR));
    
    switch(netrans_hdr->netrans_type) {
        case NETRANS_TYPE_SEND:
            printf("SEND from n%d to n%d\n", netrans_hdr->netrans_src + 1, netrans_hdr->netrans_dest + 1);
            break;
        case NETRANS_TYPE_RECEIVE:
            printf("RECEIVE from n%d to n%d\n", netrans_hdr->netrans_src + 1, netrans_hdr->netrans_dest + 1);
            break;
        case NETRANS_TYPE_ACK:
            printf("ACK from n%d to n%d\n", netrans_hdr->netrans_src + 1, netrans_hdr->netrans_dest + 1);
            break;
        case NETRANS_TYPE_CHUNK:
            printf("CHUNK from n%d to n%d\n", netrans_hdr->netrans_src + 1, netrans_hdr->netrans_dest + 1);
            break;
    }

    return 1;
}
