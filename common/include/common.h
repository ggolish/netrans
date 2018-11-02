#ifndef NETRANS_COMMON_H
#define NETRANS_COMMON_H

#include "error.h"
#include "packet.h"
#include "chunk.h"
#include "send.h"
#include "ack.h"

#define NETRANS_LOOPBACK_ID 255
#define DEFAULT_NET_DEVICE "eth0"

extern int netrans_init(char *net_device, int loopback);
extern int send_packet(int sockfd, int machine, char *packet, int packet_size, uint8_t type);
extern int machine_lookup(uint8_t *mac_addr);
extern char *receive_packet(int sockfd);

#endif /* end of include guard: NETRANS_COMMON_H */
