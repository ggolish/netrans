#ifndef NETRANS_NETRANS_H_
#define NETRANS_NETRANS_H_

extern int netrans_init(char *net_device, int loopback);
extern int netrans_send(int sockfd, int machine, char *local_path, char *remote_path);
extern int netrans_receive(int sockfd, int machine, char *local_path, char *remote_path);

#endif
