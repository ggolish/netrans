#include "netrans.h"
#include "error.h"
#include "packet.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define DEFAULT_NET_DEVICE "eth0"
#define MAX_ETHER_PAYLOAD 1500
#define MAC_ADDR_LEN 6
#define CHUNK 2

// A global ethernet packet header for sending messages
static PACKET_ETH_HDR eth_hdr;
static struct sockaddr_ll dest_addr;

// The MAC addresses for machines on network
static uint8_t mac_addrs[3][MAC_ADDR_LEN] = {
    {0x18, 0x03, 0x73, 0xd1, 0xd6, 0x29}, // n1
    {0x18, 0x03, 0x73, 0xd3, 0x63, 0x25}, // n2
    {0x18, 0x03, 0x73, 0xd1, 0xd5, 0x28}  // n3
};

static int send_message(int sockfd, int machine, const char *message, int message_size);

int netrans_init(char *net_device, int loopback)
{
    int sockfd;
    struct ifreq ifr_nd, ifr_ma;

    // Open a raw socket
    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if(sockfd == -1) {
        sprintf(err_msg, "Unable to open raw socket");
        return -1;
    }

    // Determine the device index of the given network device
    if(!net_device) net_device = strdup(DEFAULT_NET_DEVICE);
    memset(&ifr_nd, 0, sizeof(struct ifreq));
    strcpy(ifr_nd.ifr_name, net_device);
    if(ioctl(sockfd, SIOCGIFINDEX, &ifr_nd) < 0) {
        sprintf(err_msg, "Improper device name '%s'", net_device);
        return -1;
    }

    // Store network device index in destination addr struct
    dest_addr.sll_ifindex = ifr_nd.ifr_ifindex;

    // Determine the MAC address of the given network device
    memset(&ifr_ma, 0, sizeof(struct ifreq));
    strcpy(ifr_ma.ifr_name, net_device);
    if(ioctl(sockfd, SIOCGIFHWADDR, &ifr_ma) < 0) {
        sprintf(err_msg, "Unable to determine MAC address of '%s'", net_device);
        return -1;
    }

    // Initialize ethernet header and set source MAC address and ethertype
    memset(&eth_hdr, 0, sizeof(PACKET_ETH_HDR));
    if(loopback) memcpy(&eth_hdr.eth_mac_dest, &ifr_ma.ifr_hwaddr.sa_data, MAC_ADDR_LEN);
    memcpy(&eth_hdr.eth_mac_src, &ifr_ma.ifr_hwaddr.sa_data, MAC_ADDR_LEN);
    eth_hdr.eth_type = ntohs(ETHER_TYPE_NETRANS);

    return sockfd;
}

int netrans_send(int sockfd, int machine)
{
    FILE *fd;
    char **chunks = NULL, buffer[NETRANS_PAYLOAD_CHUNK];
    int *sizes = NULL, len = 0, cap = 0, bytes;

    if((fd = fopen("test.txt", "r")) == NULL) {
        sprintf(err_msg, "Unable to open test.txt for reading");
        return -1;
    }

    while((bytes = fread(buffer, sizeof(char), NETRANS_PAYLOAD_CHUNK, fd)) > 0) {
        if(len >= cap - 1) {
            cap = (cap == 0) ? CHUNK : cap * 2;
            chunks = (char **)realloc(chunks, cap * sizeof(char *));
            sizes = (int *)realloc(sizes, cap * sizeof(int));
        }
        chunks[len] = (char *)malloc(bytes * sizeof(char));
        memcpy(chunks[len], buffer, bytes * sizeof(char));
        sizes[len++] = bytes;
    }

    for(int i = 0; i < len; ++i) {
        if(send_message(sockfd, machine, chunks[i], sizes[i]) == -1) return -1;
        usleep(1);
        free(chunks[i]);
    }

    free(chunks);
    free(sizes);

    printf("Packets sent: %d\n", len);

    return 1;
}

static int send_message(int sockfd, int machine, const char *message, int message_size)
{
    char payload[MAX_ETHER_PAYLOAD];
    int eth_size, size = 0;

    // Set the destination MAC address in ethernet header
    if(machine >= 0) memcpy(&eth_hdr.eth_mac_dest, mac_addrs[machine], MAC_ADDR_LEN);

    // Set the destination MAC address in sockaddr struct
    dest_addr.sll_halen = MAC_ADDR_LEN;
    memcpy(&dest_addr.sll_addr, &eth_hdr.eth_mac_dest, MAC_ADDR_LEN);

    eth_size = sizeof(PACKET_ETH_HDR);

    // Build the payload
    memcpy(payload, &eth_hdr, eth_size);
    size += eth_size;
    memcpy(payload + size, message, message_size);
    size += message_size;

    int len = sendto(sockfd, payload, size, 0, (struct sockaddr *)(&dest_addr), sizeof(struct sockaddr_ll));
    if(len == -1) {
       strcpy(err_msg, strerror(errno));
       return -1;
    }

    return 0;
}
