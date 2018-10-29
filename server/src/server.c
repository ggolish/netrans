#include "server.h"
#include "common.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define K 1024

static int process_packet(int sockfd, PACKET_NETRANS_HDR *netrans_hdr);
static int process_send(int sockfd, PACKET_NETRANS_SEND *send);

int netransd_mainloop(int sockfd)
{
    pid_t pid;

    for(;;) {
        PACKET_NETRANS_HDR *packet = receive_packet(sockfd);

        if(packet != NULL) {
            pid = fork();
            if(pid == -1) {
                sprintf(err_msg, "Unable to fork a new process");
                return -1;
            } else if(pid == 0) {
                process_packet(sockfd, packet);
                free(packet);
            } else {
                free(packet);
            }
        }
    }
}

static int process_packet(int sockfd, PACKET_NETRANS_HDR *netrans_hdr)
{
    switch(netrans_hdr->netrans_type) {
        case NETRANS_TYPE_SEND:
            printf("SEND from n%d to n%d\n", netrans_hdr->netrans_src + 1, netrans_hdr->netrans_dest + 1);
            process_send(sockfd, (PACKET_NETRANS_SEND *)(netrans_hdr + sizeof(PACKET_NETRANS_HDR)));
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

static int process_send(int sockfd, PACKET_NETRANS_SEND *send)
{
    printf("File size: %d\n", send->send_file_sz);
    printf("Path size: %d\n", send->send_path_sz);
    printf("Path: ");
    fwrite(send->send_path, sizeof(char), send->send_path_sz, stdout);
    printf("\n\n");
    return 1;
}
