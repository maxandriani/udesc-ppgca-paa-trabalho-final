#ifndef WEBLIST_PRI_H
#define WEBLIST_PRI_H

#include "weblist_pub.h"
#include <memory.h>
#include <stdlib.h>
#include <math.h>

typedef union leaf_t {
    weblist_p node;
    pDDLL list;
} leaf_t;

typedef struct weblist_t {
    size_t depth;
    size_t count[8];
    leaf_t leafs[8];
    size_t data_size;
} weblist_t;

int _create_leaf_node(weblist_p root, size_t level);
int _destroy_leaf_node(weblist_p root, size_t level);
int _calc_number_of_nodes_by_level(int level);
size_t _calc_total_count(size_t *count);

#endif