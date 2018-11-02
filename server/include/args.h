#ifndef NETRANS_ARGS_H
#define NETRANS_ARGS_H

typedef struct {
    char *net_device;
} netrans_args_t;

extern netrans_args_t *args_process(int argc, char *argv[]);
extern void args_print(netrans_args_t *args);

#endif
