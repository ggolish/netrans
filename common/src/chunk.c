
#include "chunk.h"

#include <endian.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#define BREAK 1

int chunks_send(int sockfd, int machine, FILE *fd)
{
    PACKET_NETRANS_CHUNK chunk;
    char buffer[NETRANS_PAYLOAD_CHUNK];
    int nbytes, size;
    uint64_t i;

    i = 0;
    while((nbytes = fread(buffer, sizeof(char), NETRANS_PAYLOAD_CHUNK, fd)) > 0) {
        memset(&chunk, 0, sizeof(PACKET_NETRANS_CHUNK));
        chunk.chunk_id = htobe64(i);
        chunk.chunk_size = htons(nbytes);
        memcpy(chunk.chunk_payload, buffer, nbytes);
        size = sizeof(PACKET_NETRANS_CHUNK) - (NETRANS_PAYLOAD_CHUNK - nbytes);
        if(send_packet(sockfd, machine, (char *)(&chunk), size, NETRANS_TYPE_CHUNK) == -1)
            return -1;
        usleep(BREAK);
        i++;
    }

    return 1;
}
