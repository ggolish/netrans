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

#define K 1024

static PACKET_ETH_HDR eth_hdr;
static unsigned char n2_mac_addr[] = {0x18, 0x03, 0x73, 0xd3, 0x63, 0x25};

int netrans_init(char *net_device)
{
    int sockfd;
    struct ifreq ifr_nd, ifr_ma;
    struct sockaddr_ll addr;
    char buffer[4 * K];

    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if(sockfd == -1) {
        sprintf(err_msg, "Unable to open raw socket");
        return -1;
    }

    memset(&ifr_nd, 0, sizeof(struct ifreq));
    strcpy(ifr_nd.ifr_name, net_device);
    if(ioctl(sockfd, SIOCGIFINDEX, &ifr_nd) < 0) {
        sprintf(err_msg, "Improper device name '%s'", net_device);
        return -1;
    }
    
    memset(&ifr_ma, 0, sizeof(struct ifreq));
    strcpy(ifr_ma.ifr_name, net_device);
    if(ioctl(sockfd, SIOCGIFHWADDR, &ifr_ma) < 0) {
        sprintf(err_msg, "Unable to determine MAC address of '%s'", net_device);
        return -1;
    }
    
    memset(&eth_hdr, 0, sizeof(PACKET_ETH_HDR));
//     memcpy(&eth_hdr.eth_mac_dest, &ifr_ma.ifr_hwaddr.sa_data, 6);
    memcpy(&eth_hdr.eth_mac_dest, n2_mac_addr, 6);
    memcpy(&eth_hdr.eth_mac_src, &ifr_ma.ifr_hwaddr.sa_data, 6);
    eth_hdr.eth_type = ntohs(ETHER_TYPE_NETRANS);

    memcpy(buffer, &eth_hdr, sizeof(PACKET_ETH_HDR));
    memcpy(buffer + sizeof(PACKET_ETH_HDR), "Hello", 5);

    addr.sll_ifindex = ifr_nd.ifr_ifindex;
    addr.sll_halen = 6;
    memcpy(&addr.sll_addr, n2_mac_addr, 6);

    int len = sendto(sockfd, buffer, 100, 0, (struct sockaddr *)(&addr), sizeof(struct sockaddr_ll));
    if(len == -1) {
       strcpy(err_msg, strerror(errno));
       return -1;
    }

    return sockfd;
}
