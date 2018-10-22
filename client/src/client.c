
#include "client.h"
#include "common.h"

#include <stdio.h>

int netrans_send(int sockfd, int machine, char *local_path, char *remote_path)
{
    FILE *fd;
    PACKET_NETRANS_CHUNK **chunks;
    int len;

    if((fd = fopen(local_path, "r")) == NULL) {
        sprintf(err_msg, "Unable to open test.txt for reading");
        return -1;
    }

    chunks = chunks_from_file(fd, &len);
    if(chunks_send(sockfd, machine, chunks, len) == -1) return -1;
    chunks_destroy(chunks, len);

    printf("Packets sent: %d\n", len);

    return 1;
}

int netrans_receive(int sockfd, int machine, char *local_path, char *remote_path)
{
    return 0;
}
