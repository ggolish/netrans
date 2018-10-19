#include "chunk.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define CHUNK 2

static PACKET_NETRANS_CHUNK *new_chunk(int id, char *payload, int sz);

PACKET_NETRANS_CHUNK **chunks_from_file(FILE *fd, int *len_r)
{
    PACKET_NETRANS_CHUNK **chunks = NULL;
    char buffer[NETRANS_PAYLOAD_CHUNK];
    int bytes;
    int len = 0, cap = 0;

    while((bytes = fread(buffer, sizeof(char), NETRANS_PAYLOAD_CHUNK, fd)) > 0) {
        if(len >= cap - 1) {
            cap = (cap == 0) ? CHUNK : cap * 2;
            chunks = (PACKET_NETRANS_CHUNK **)realloc(chunks, cap * sizeof(PACKET_NETRANS_CHUNK *));
        }
        chunks[len] = new_chunk(len, buffer, bytes);
        len++;
    }

    *len_r = len;
    return chunks;
}

static PACKET_NETRANS_CHUNK *new_chunk(int id, char *payload, int sz)
{
    PACKET_NETRANS_CHUNK *tmp;

    tmp = (PACKET_NETRANS_CHUNK *)malloc(sizeof(PACKET_NETRANS_CHUNK));
    memset(tmp, 0, sizeof(PACKET_NETRANS_CHUNK));
    tmp->chunk_id = id;
    tmp->chunk_size = sz;
    memcpy(tmp->chunk_payload, payload, sz * sizeof(uint8_t));
    return tmp;
}
