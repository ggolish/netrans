#include "server.h"
#include "common.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <stdio.h>
#include <string.h>

#define K 1024

static int process_packet(PACKET_NETRANS_HDR *netrans_hdr);

int netransd_mainloop(int sockfd)
{
    for(;;) {
        PACKET_NETRANS_HDR *packet = receive_packet(sockfd);
        if(packet == NULL) return -1;
        process_packet(packet);
    }
}

static int process_packet(PACKET_NETRANS_HDR *netrans_hdr)
{
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
