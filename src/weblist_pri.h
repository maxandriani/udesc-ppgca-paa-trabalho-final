#ifndef WEBLIST_PRI_H
#define WEBLIST_PRI_H

#include "weblist_pub.h"
#include <memory.h>
#include <stdlib.h>
#include <math.h>

typedef struct list_t *list_p, **list_pp;

typedef struct list_t {
    weblist_p root;
    int key;
    size_t count;
    size_t data_size;
    pDDLL data;
    list_p next;
    list_p prev;
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
    size_t data_size;
} weblist_t;

int _is_leaf_node(weblist_p root);
int _search_by_value(list_p list, void *value, compare_fn cmp);
int _search_and_delete_by_value(list_p list, void *value, compare_fn cmp);
int _find_min_value(list_p list, void *value, compare_fn cmp);
int _find_max_value(list_p list, void *value, compare_fn cmp);
int _create_leaf_node(list_pp list, size_t data_size);
int _create_node(weblist_pp root, list_pp last, size_t level, size_t depth, size_t data_size);
int _destroy_leaf_node(list_pp list);
int _destroy_node(weblist_pp root);
void _shift_left(list_p list, compare_fn cmp);
void _shift_right(list_p list, compare_fn cmp);
void _update_index(weblist_p node, int key, void * element);
void _balance(weblist_p root, compare_fn cmp);
size_t _calc_insert_idx(weblist_p root, void *data, compare_fn cmp);
int _add_data(weblist_p root, void *data, compare_fn cmp);
int _remove_data(weblist_p root, void *data, compare_fn cmp);
int _search_data(weblist_p root, void *data, compare_fn cmp);
int _walk_ddll_list(list_p list, process_fn cb);
int _walk_node(weblist_p root, process_fn cb);
int _get_list_by_key(weblist_p root, list_pp list, int key);
list_p _get_leaftish_leaf(weblist_p root);
int _remove_list_data_by_value(list_p list, void *value, compare_fn cmp);

#endif