#ifndef PACKETS_H_
#define PACKETS_H_

#include <stdint.h>

#define ETHER_TYPE_NETRANS 0x0929

// Ethernet packet header
typedef struct __attribute__((packed)) {
    uint8_t  eth_mac_dest[6]; // The destination MAC address
    uint8_t  eth_mac_src[6];  // The source MAC address
    uint16_t eth_type;        // The type of the payload
} PACKET_ETH_HDR;

#define NETRANS_TYPE_SEND     0x01
#define NETRANS_TYPE_RECEIVE  0x02
#define NETRANS_TYPE_ACK      0x03

typedef struct __attribute__((packed)) {
    uint8_t netrans_src;
    uint8_t netrans_dest;
    uint8_t netrans_type;
} PACKET_NETRANS_HDR;

#define NETRANS_PAYLOAD_CHUNK 512

#endif
