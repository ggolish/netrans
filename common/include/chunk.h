#ifndef NETRANS_CHUNK_H_
#define NETRANS_CHUNK_H_

#include "common.h"

#include <stdio.h>

extern int chunks_send(int sockfd, int machine, FILE *fd, uint64_t file_size, int verbose);

#endif
