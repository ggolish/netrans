#ifndef NETRANS_CHUNK_H_
#define NETRANS_CHUNK_H_

#include "packet.h"

#include <stdio.h>

extern PACKET_NETRANS_CHUNK **chunks_from_file(FILE *fd, int *len_r);
extern int chunks_send(int sockfd, int machine, PACKET_NETRANS_CHUNK **chunks, int nchunks);
extern void chunks_destroy(PACKET_NETRANS_CHUNK **chunks, int nchunks);

#endif
