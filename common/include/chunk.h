#ifndef NETRANS_CHUNK_H_
#define NETRANS_CHUNK_H_

#include "packet.h"

#include <stdio.h>

extern PACKET_NETRANS_CHUNK **chunks_from_file(FILE *fd, int *len_r);

#endif
