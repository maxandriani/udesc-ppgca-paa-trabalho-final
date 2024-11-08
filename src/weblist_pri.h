#ifndef WEBLIST_PRI_H
#define WEBLIST_PRI_H

#include "weblist_pub.h"
#include <memory.h>
#include <stdlib.h>
#include <math.h>

typedef struct list_t *list_p, **list_pp;

typedef struct list_t {
    int key;
    size_t count;
    size_t data_size;
    pDDLL data;
    list_p next;
    list_p prev;
    weblist_p root;
} list_t;

typedef union leaf_t {
    weblist_p node;
    list_p list;
} leaf_t;

typedef struct weblist_t {
    weblist_p root;
    size_t level;
    size_t depth;
    leaf_t leafs[8];
    void * boundaries[8];
} weblist_t;

#endif