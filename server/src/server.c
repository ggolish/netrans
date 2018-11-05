#include "server.h"
#include "queue.h"
#include "common.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define K 1024
#define RECEIVE_TIMEOUT 5

// Server states
#define SERVER_NORMAL       1   // Simply waiting for packets
#define SERVER_RECEIVING    2   // Waiting to receive a file from a remote client
#define SERVER_SENDING      3   // Waiting for acknowledgement to send file

typedef struct {

    // The current state of the server
    uint8_t state;
    time_t last_time;

    // Variables for writing files sent to server
    char *send_path;
    int fd;
    int total_size;
    int total_chunks;
    int current_size;
    int current_chunks;

    // The current remote machine to send to / receive from
    int remote_machine;

} server_t;

static server_t server;

static int process_packet(int sockfd, char *packet);
static int process_send(int sockfd, int remote_machine, PACKET_NETRANS_SEND *send);
static int process_chunk(PACKET_NETRANS_CHUNK *chunk);

int netransd_mainloop(int sockfd)
{
    char *packet;
    time_t current_time;

    memset(&server, 0, sizeof(server_t));
    server.state = SERVER_NORMAL;

    for(;;) {
        packet = (!packet_empty() && server.state == SERVER_NORMAL) ? packet_pop() : receive_packet(sockfd);
        if(packet != NULL) {
            process_packet(sockfd, packet);
        }

        if(server.state == SERVER_RECEIVING) {
            current_time = time(NULL);
            if(current_time - server.last_time > RECEIVE_TIMEOUT) {
                fprintf(stderr, "Warning: Packet receive timeout\n");
                close(server.fd);
                unlink(server.send_path);
                free(server.send_path);
                server.state = SERVER_NORMAL;
            }
        }
      }
}

static int process_packet(int sockfd, char *packet)
{
    PACKET_NETRANS_HDR *netrans_hdr;
    int offset;

    offset = sizeof(PACKET_ETH_HDR);
    netrans_hdr = (PACKET_NETRANS_HDR *)(packet + offset);
    offset += sizeof(PACKET_NETRANS_HDR);

    switch(netrans_hdr->netrans_type) {
        case NETRANS_TYPE_SEND:
            if(server.state != SERVER_NORMAL) {
                packet_push(packet);
            } else {
                process_send(sockfd, netrans_hdr->netrans_src, (PACKET_NETRANS_SEND *)(packet + offset));
                free(packet);
            }
            break;
        case NETRANS_TYPE_RECEIVE:
            printf("RECEIVE from n%d to n%d\n", netrans_hdr->netrans_src + 1, netrans_hdr->netrans_dest + 1);
            free(packet);
            break;
        case NETRANS_TYPE_ACK:
            printf("ACK from n%d to n%d\n", netrans_hdr->netrans_src + 1, netrans_hdr->netrans_dest + 1);
            free(packet);
            break;
        case NETRANS_TYPE_CHUNK:
            if(server.state == SERVER_RECEIVING && server.remote_machine == netrans_hdr->netrans_src) {
                process_chunk((PACKET_NETRANS_CHUNK *)(packet + offset));
            }
            free(packet);
            break;
    }

    return 1;
}

static int process_send(int sockfd, int remote_machine, PACKET_NETRANS_SEND *send)
{
    int file_size;

    server.send_path = (char *)malloc(send->send_path_sz * sizeof(char) + 1);
    memcpy(server.send_path, send->send_path, send->send_path_sz);
    server.send_path[send->send_path_sz] = '\0';
    file_size = ntohl(send->send_file_sz);

    if((server.fd = open(server.send_path, O_CREAT | O_WRONLY, 0644)) == -1) {
        fprintf(stderr, "cannot open %s\n", server.send_path);
        free(server.send_path);
        return acknowledge(sockfd, remote_machine, NETRANS_ACK_NO);
    }

    for(int i = 0; i < file_size; ++i) {
        if(write(server.fd, "\0", 1) == -1) {
            fprintf(stderr, "cannot write to %s\n", server.send_path);
            return acknowledge(sockfd, remote_machine, NETRANS_ACK_NO);
        }
    }

    server.total_size = file_size;
    server.total_chunks = (file_size % NETRANS_PAYLOAD_CHUNK == 0) ?
        file_size / NETRANS_PAYLOAD_CHUNK : file_size / NETRANS_PAYLOAD_CHUNK + 1;
    server.current_size = server.current_chunks = 0;
    server.remote_machine = remote_machine;
    server.state = SERVER_RECEIVING;
    server.last_time = time(NULL);

    return acknowledge(sockfd, remote_machine, NETRANS_ACK_YES);
}

static int process_chunk(PACKET_NETRANS_CHUNK *chunk)
{
    int size, offset;

    size = ntohs(chunk->chunk_size);
    offset = ntohl(chunk->chunk_id) * NETRANS_PAYLOAD_CHUNK;
    server.last_time = time(NULL);

    lseek(server.fd, offset, SEEK_SET);
    write(server.fd, chunk->chunk_payload, size);

    server.current_size += size;
    server.current_chunks++;

    if(server.current_size == server.total_size && server.current_chunks == server.total_chunks) {
        free(server.send_path);
        close(server.fd);
        server.state = SERVER_NORMAL;
    } else if((server.current_size >= server.total_size) || (server.current_chunks >= server.total_chunks)) {
        fprintf(stderr, "NOPE!\n");
        close(server.fd);
        unlink(server.send_path);
        free(server.send_path);
        server.state = SERVER_NORMAL;
    }

    return 1;
}
