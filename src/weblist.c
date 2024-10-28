#include "weblist_pri.h"

int weblist_create (weblist_pp pp_weblist, size_t depth, size_t data_size) {
    return FAIL;
}

int weblist_destruct (weblist_pp pp_weblist, compare_fn cmp) {
    return FAIL;
}

int weblist_add_data(weblist_p webslit, void *data) {
    return FAIL;
}

int weblist_remove_data(weblist_p weblist, void *data) {
    return FAIL;
}

int weblist_search_data(weblist_p weblist, void *data, compare_fn cmp) {
    return FAIL;
}

int weblist_walk_data(weblist_p weblist, process_fn cb) {
    return FAIL;
}

int weblist_copy_list_by_key(weblist_p weblist, int key, ppDDLL list) {
    return FAIL;
}

int weblist_replace_list_by_key(weblist_p weblist, int key, pDDLL list) {
    return FAIL;
}

int weblist_remove_list_by_key(weblist_p weblist, int key, ppDDLL list) {
    return FAIL;
}

int weblist_create_list_by_key(weblist_p weblist, int key) {
    return FAIL;
}

int weblist_count_by_key(weblist_p weblist, int key, int *count) {
    return FAIL;
}

int weblist_total_of_keys(weblist_p weblist, int *count) {
    return FAIL;
}

int weblist_count(weblist_p weblist, int *count) {
    return FAIL;
}

int weblist_get_keys(weblist_p weblist, ppDDLL keys) {
    return FAIL;
}

int weblist_is_balanced(weblist_p weblist) {
    return FAIL;
}
