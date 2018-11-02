#include "server.h"
#include "common.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define K 1024

static void reap(int sig);
static int process_packet(int sockfd, char *packet);
static int process_send(int sockfd, PACKET_NETRANS_SEND *send);

int netransd_mainloop(int sockfd)
{
    char *packet;
    pid_t pid;

    signal(SIGCHLD, reap);

    for(;;) {
        packet = receive_packet(sockfd);

        if(packet != NULL) {
            pid = fork();
            if(pid == -1) {
                sprintf(err_msg, "Unable to fork a new process");
                return -1;
            } else if(pid == 0) {
                if(process_packet(sockfd, packet) == -1) {
                    return -1;
                }
                free(packet);
                return 1;
            } else {
                free(packet);
            }
        }
      }
}

static void reap(int sig)
{
    int exit_status;
    pid_t pid;

    pid = wait(&exit_status);
    printf("%d exited: %d\n", pid, exit_status);
}

static int process_packet(int sockfd, char *packet)
{
    PACKET_NETRANS_HDR *netrans_hdr;
    int offset;

    offset = sizeof(PACKET_ETH_HDR);
    netrans_hdr = (PACKET_NETRANS_HDR *)(packet + offset);
    offset += sizeof(PACKET_NETRANS_HDR);

    switch(netrans_hdr->netrans_type) {
        case NETRANS_TYPE_SEND:
            printf("SEND from n%d to n%d\n", netrans_hdr->netrans_src + 1, netrans_hdr->netrans_dest + 1);
            process_send(sockfd, (PACKET_NETRANS_SEND *)(packet + offset));
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
    printf("File size: %d\n", ntohl(send->send_file_sz));
    printf("Path size: %d\n", send->send_path_sz);
    printf("Path: ");
    fwrite(send->send_path, sizeof(char), send->send_path_sz, stdout);
    printf("\n\n");
    return 1;
}
