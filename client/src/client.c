
#include "client.h"
#include "common.h"

#include <stdio.h>
#include <string.h>

int netrans_send(int sockfd, int machine, char *local_path, char *remote_path, int verbose)
{
    FILE *fd;
    uint64_t file_size;
    int reply;

    if((fd = fopen(local_path, "r")) == NULL) {
        sprintf(err_msg, "Unable to open '%s' for reading", local_path);
        return -1;
    }

    if(verbose) printf("Opened '%s' for sending.\n", local_path);

    // Compute the size of the file to send
    fseek(fd, 0, SEEK_END);
    file_size = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    if(verbose) printf("Sending a SEND request to server.\n");

    // Wait for server to acknowledge before sending packet
    reply = send_request(sockfd, machine, file_size, strlen(remote_path), (unsigned char *)remote_path);

    if(verbose) printf("Received ACK from server.\n");

    switch(reply) {
        case NETRANS_ACK_YES:
            break;
        case NETRANS_ACK_NO:
            sprintf(err_msg, "Send request denied");
            return -1;
        case -1:
            return -1;
    }

    if(verbose) printf("Sending chunks to server...\n");

    return chunks_send(sockfd, machine, fd, file_size, verbose);
}

int netrans_receive(int sockfd, int machine, char *local_path, char *remote_path)
{
    return 0;
}
