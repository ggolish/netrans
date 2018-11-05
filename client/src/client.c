
#include "client.h"
#include "common.h"

#include <stdio.h>
#include <string.h>

int netrans_send(int sockfd, int machine, char *local_path, char *remote_path)
{
    FILE *fd;
    int file_size, reply;

    if((fd = fopen(local_path, "r")) == NULL) {
        sprintf(err_msg, "Unable to open test.txt for reading");
        return -1;
    }

    // Compute the size of the file to send
    fseek(fd, 0, SEEK_END);
    file_size = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    // Wait for server to acknowledge before sending packet
    reply = send_request(sockfd, machine, file_size, strlen(remote_path), (unsigned char *)remote_path);
    switch(reply) {
        case NETRANS_ACK_YES:
            break;
        case NETRANS_ACK_NO:
            sprintf(err_msg, "Send request denied");
            return -1;
        case -1:
            return -1;
    }

    return chunks_send(sockfd, machine, fd);
}

int netrans_receive(int sockfd, int machine, char *local_path, char *remote_path)
{
    return 0;
}
