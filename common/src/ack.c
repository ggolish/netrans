
#include "ack.h"

#include <string.h>

int acknowledge(int sockfd, int machine, uint8_t ack_ok)
{
    PACKET_NETRANS_ACK ack;

    memset(&ack, 0, sizeof(PACKET_NETRANS_ACK));
    ack.ack_ok = ack_ok;

    return send_packet(sockfd, machine, (char *)(&ack), sizeof(PACKET_NETRANS_ACK), NETRANS_TYPE_ACK);
}
