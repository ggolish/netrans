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

#define MAX_ETHER_PAYLOAD 1500
#define MAC_ADDR_LEN 6

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

int netrans_init(char *net_device)
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
    memcpy(&eth_hdr.eth_mac_src, &ifr_ma.ifr_hwaddr.sa_data, MAC_ADDR_LEN);
    eth_hdr.eth_type = ntohs(ETHER_TYPE_NETRANS);

    // Attempt to send some message
    send_message(sockfd, 2, "Hello from n1", 13);

    return sockfd;
}

static int send_message(int sockfd, int machine, const char *message, int message_size)
{
    char payload[MAX_ETHER_PAYLOAD];

    // Set the destination MAC address in ethernet header
    memcpy(&eth_hdr.eth_mac_dest, mac_addrs[machine], MAC_ADDR_LEN);

    // Set the destination MAC address in sockaddr struct
    dest_addr.sll_halen = MAC_ADDR_LEN;
    memcpy(&dest_addr.sll_addr, mac_addrs[machine], MAC_ADDR_LEN);

    // Build the payload
    memcpy(payload, &eth_hdr, sizeof(PACKET_ETH_HDR));
    memcpy(payload, message, message_size);

    int len = sendto(sockfd, payload, MAX_ETHER_PAYLOAD, 0, (struct sockaddr *)(&dest_addr), sizeof(struct sockaddr_ll));
    if(len == -1) {
       strcpy(err_msg, strerror(errno));
       return -1;
    }

    return 0;
}
