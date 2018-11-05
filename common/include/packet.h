#ifndef PACKETS_H_
#define PACKETS_H_

#include <stdint.h>

#define ETH_TYPE_NETRANS 0x0929

// Ethernet packet header
typedef struct __attribute__((packed)) {
    uint8_t  eth_mac_dest[6]; // The destination MAC address
    uint8_t  eth_mac_src[6];  // The source MAC address
    uint16_t eth_type;        // The type of the payload
} PACKET_ETH_HDR;

#define NETRANS_TYPE_SEND     0x01
#define NETRANS_TYPE_RECEIVE  0x02
#define NETRANS_TYPE_ACK      0x03
#define NETRANS_TYPE_CHUNK    0x04

typedef struct __attribute__((packed)) {
    uint8_t netrans_src;
    uint8_t netrans_dest;
    uint8_t netrans_type;
} PACKET_NETRANS_HDR;

#define NETRANS_PAYLOAD_CHUNK 1024

/*
    Payload will be a path to store the file
*/
typedef struct __attribute__((packed)) {
    uint32_t send_file_sz;
    uint8_t send_path_sz;
    uint8_t send_path[NETRANS_PAYLOAD_CHUNK];
} PACKET_NETRANS_SEND;

/*
    Payload will be a path to target file
*/
typedef struct __attribute__((packed)) {
    uint8_t receive_path_size;
} PACKET_NETRANS_RECEIVE;

#define NETRANS_ACK_YES  1
#define NETRANS_ACK_NO   0

typedef struct __attribute__((packed)) {
    uint8_t ack_ok;
} PACKET_NETRANS_ACK;

typedef struct __attribute__((packed)) {
    uint32_t chunk_id;
    uint16_t chunk_size;
    uint8_t chunk_payload[NETRANS_PAYLOAD_CHUNK];
} PACKET_NETRANS_CHUNK;

#endif
