
#include "queue.h"

#include <stdlib.h>

typedef struct node {
    char *packet;
    struct node *next;
} node_t;

static node_t *head = NULL;
static node_t *tail = NULL;

static node_t *new_node(char *packet);

void packet_push(char *packet)
{
    node_t *n;

    n = new_node(packet);

    if(packet_empty()) {
        head = tail = n;
    } else {
        tail->next = n;
        tail = n;
    }
}

char *packet_pop()
{
    node_t *n;
    char *packet;

    if(packet_empty()) return NULL;
    n = head;
    packet = n->packet;
    head = head->next;
    if(head == NULL) tail = NULL;
    free(n);
    return packet;
}

int packet_empty()
{
    return ((head == NULL) && (tail == NULL));
}

static node_t *new_node(char *packet)
{
    node_t *n;

    n = (node_t *)malloc(sizeof(node_t));
    n->packet = packet;
    n->next = NULL;
    return n;
}
