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

static PACKET_ETH_HDR eth_hdr;

int netrans_init(char *net_device)
{
    int sockfd;
    struct ifreq ifr;

    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if(sockfd == -1) {
        sprintf(err_msg, "Unable to open raw socket");
        return -1;
    }

    memset(&ifr, 0, sizeof(struct ifreq));
    strcpy(ifr.ifr_name, net_device);
    if(ioctl(sockfd, SIOCGIFINDEX, &ifr) < 0) {
        sprintf(err_msg, "Improper device name '%s'", net_device);
        return -1;
    }

    memset(&eth_hdr, 0, sizeof(PACKET_ETH_HDR));
    memset(&eth_hdr.eth_mac_dest, 0xFF, 6);
    memcpy(&eth_hdr.eth_mac_src, &ifr.ifr_hwaddr.sa_data, 6);
    eth_hdr.eth_type = ntohs(ETHER_TYPE_NETRANS);

    int len = send(sockfd, &eth_hdr, sizeof(PACKET_ETH_HDR), 0);
    printf("%d\n", len);

    return sockfd;
}
