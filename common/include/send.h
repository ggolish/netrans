#ifndef NETRANS_SEND_H
#define NETRANS_SEND_H

#include "common.h"

extern int send_request(int sockfd, int machine, uint64_t size, uint8_t path_len, uint8_t *path);

#endif /* end of include guard: NETRANS_SEND_H */
