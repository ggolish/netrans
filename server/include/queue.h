#ifndef NETRANS_QUEUE_H
#define NETRANS_QUEUE_H

extern void packet_push(char *packet);
extern char *packet_pop();
extern int packet_empty();

#endif /* end of include guard: NETRANS_QUEUE_H */
