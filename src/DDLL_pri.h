#ifndef DDLL_PRI_H
#define DDLL_PRI_H

#include "DDLL_pub.h"
#include <memory.h>
#include <stdlib.h>

typedef struct node_s {
    void * value;
    struct node_s * next;
    struct node_s * prev;
} node_t, *p_node_t, **pp_node_t;

typedef struct DDLL {
    node_t * head;
    node_t * tail;
    size_t data_size;
    size_t length;
} DDLL;

int _ddll_create_node(pDDLL p_ddll, pp_node_t pp_node, p_node_t prev, p_node_t next, void * value);
int _ddll_remove_node(pDDLL p_ddll, p_node_t p_node, void * value);

#endif