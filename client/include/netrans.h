#ifndef NETRANS_NETRANS_H_
#define NETRANS_NETRANS_H_

extern int netrans_init(char *net_device, int loopback);
extern int netrans_send(int sockfd, int machine);

#endif
