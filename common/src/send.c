
#include "send.h"

#include <string.h>
#include <time.h>

#define MIN(X, Y) ((X < Y) ? X : Y)

#define SEND_REQUEST_TIMEOUT 30

int send_request(int sockfd, int machine, uint32_t file_size, uint8_t path_len, uint8_t *path)
{
    PACKET_NETRANS_SEND send;
    PACKET_NETRANS_HDR *reply;
    PACKET_NETRANS_ACK *ack;
    int size;
    time_t start, current;

    memset(&send, 0, sizeof(PACKET_NETRANS_SEND));
    send.send_file_sz = file_size;
    send.send_path_sz = path_len;
    memcpy(&send.send_path, path, MIN(path_len, NETRANS_PAYLOAD_CHUNK));
    size = sizeof(PACKET_NETRANS_SEND) - (NETRANS_PAYLOAD_CHUNK - path_len);

    if(send_packet(sockfd, machine, (char *)(&send), size, NETRANS_TYPE_SEND) == -1) {
        return -1;
    }

    start = time(NULL);
    for(;;) {
        reply = receive_packet(sockfd);
        if(reply->netrans_type == NETRANS_TYPE_ACK) {
            if(reply->netrans_src == machine) {
                ack = (PACKET_NETRANS_ACK *)(reply + sizeof(PACKET_NETRANS_HDR));
                return ack->ack_ok;
            }
        }
        current = time(NULL);
        if((current - start) >= SEND_REQUEST_TIMEOUT) {
            sprintf(err_msg, "Send request timeout");
            return -1;
        }
    }
}
