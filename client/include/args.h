#ifndef NETRANS_ARGS_H
#define NETRANS_ARGS_H

typedef struct {
    char *net_device;
    int loopback;
    int target_machine;
} netrans_args_t;

extern netrans_args_t *args_process(int argc, char *argv[]);

#endif
