#ifndef NETRANS_ERROR_H_
#define NETRANS_ERROR_H_

#define MAX_ERROR 1024

char err_msg[MAX_ERROR];

extern void die(int exit_code);
extern void noerror();

#endif /* end of include guard: NETRANS_ERROR_H_ */
