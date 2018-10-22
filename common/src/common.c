#include "common.h"

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
#define NETRANS_MACHINES 3

// A global ethernet packet header for sending messages
static PACKET_ETH_HDR eth_hdr;
static PACKET_NETRANS_HDR netrans_hdr;
static struct sockaddr_ll dest_addr;

// The MAC addresses for machines on network
static uint8_t mac_addrs[NETRANS_MACHINES][MAC_ADDR_LEN] = {
    {0x18, 0x03, 0x73, 0xd1, 0xd6, 0x29}, // n1
    {0x18, 0x03, 0x73, 0xd3, 0x63, 0x25}, // n2
    {0x18, 0x03, 0x73, 0xd1, 0xd5, 0x28}  // n3
};

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
    dest_addr.sll_halen = MAC_ADDR_LEN;

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
    eth_hdr.eth_type = ntohs(ETH_TYPE_NETRANS);

    // Initialize netrans header and set source / dest machine
    memset(&netrans_hdr, 0, sizeof(PACKET_NETRANS_HDR));
    netrans_hdr.netrans_src = (uint8_t)machine_lookup(eth_hdr.eth_mac_src);
    if(loopback) netrans_hdr.netrans_dest = netrans_hdr.netrans_src;

    return sockfd;
}

int send_packet(int sockfd, int machine, char *packet, int packet_size, uint8_t type)
{
    char payload[MAX_ETHER_PAYLOAD];
    int eth_size, netrans_size, size = 0;

    // Set the destination MAC address in ethernet header, and fill in destination of netrans header
    if(machine >= 0) {
        memcpy(&eth_hdr.eth_mac_dest, mac_addrs[machine], MAC_ADDR_LEN);
        netrans_hdr.netrans_dest = machine;
    }

    // Set the type of the netrans packet being sent
    netrans_hdr.netrans_type = type;

    // Set the destination MAC address in sockaddr struct
    memcpy(&dest_addr.sll_addr, &eth_hdr.eth_mac_dest, MAC_ADDR_LEN);

    eth_size = sizeof(PACKET_ETH_HDR);
    netrans_size = sizeof(PACKET_NETRANS_HDR);

    memcpy(payload, &eth_hdr, eth_size);
    size += eth_size;
    memcpy(payload + size, &netrans_hdr, netrans_size);
    size += netrans_size;
    memcpy(payload + size, packet, packet_size);
    size += packet_size;

    int len = sendto(sockfd, payload, size, 0, (struct sockaddr *)(&dest_addr), sizeof(struct sockaddr_ll));
    if(len == -1) {
       strcpy(err_msg, strerror(errno));
       return -1;
    }

    return 0;
}

int machine_lookup(uint8_t *mac_addr)
{
    for(int i = 0; i < NETRANS_MACHINES; ++i) {
        if(memcmp(mac_addr, mac_addrs[i], MAC_ADDR_LEN) == 0) return i;
    }
    return -1;
}
