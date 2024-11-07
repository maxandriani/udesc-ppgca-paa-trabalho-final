#include "weblist_pri.h"


int _create_leaf_node(weblist_p root, size_t level) {
    if (root == NULL) return FAIL;

    if (root->depth == level) {
        // inicializa as listas
        for (size_t i = 0; i < 8; i++) {
            if (cDDLL(&root->leafs[i].list, root->data_size) == FAIL) {
                for (size_t x = i; i > 0; i--) {
                    dDDLL(&root->leafs[x].list);
                }
                return FAIL;
            }
        }
    } else {
        // inicializa as folhas
        for (size_t i = 0; i < 8; i++) {
            root->leafs[i].node = malloc(sizeof(weblist_t));
            if (root->leafs[i].node == NULL) {
                for (size_t x = i; i > 0; i--) {
                    free(root->leafs[x].node);
                }
                return FAIL;
            }
            memset(root->leafs[i].node->count, 0, sizeof(root->leafs[i].node->count));
            root->leafs[i].node->data_size = root->data_size;
            root->leafs[i].node->depth = root->depth;

            if (_create_leaf_node(root->leafs[i].node, level + 1) == FAIL) {
                for (size_t x = i; i > 0; i--) {
                    free(root->leafs[x].node);
                }
                return FAIL;
            }
        }
    }

    return SUCCESS;
}

int _destroy_leaf_node(weblist_p root, size_t level) {
    if (root->depth == level) {
        for(size_t i = 0; i < 8; i++) {
            if (dDDLL(&root->leafs[i].list) == FAIL) {
                return FAIL;
            }
        }
    } else {
        for(size_t i = 0; i < 8; i++) {
            if (_destroy_leaf_node(root->leafs[i].node, level + 1) == FAIL) {
                return FAIL;
            }
        }
    }

    free(root);

    return SUCCESS;
}

int _calc_number_of_nodes_by_level(int level) {
    return pow(8, (level + 1));
}

int weblist_create(weblist_pp pp_weblist, size_t depth, size_t data_size) {
    if (pp_weblist == NULL || *pp_weblist != NULL || data_size <= 0) return FAIL;
    if (depth < 0) return FAIL;

    *pp_weblist = malloc(sizeof(weblist_t));
    if (!*pp_weblist) return FAIL;

    (*pp_weblist)->depth = depth;
    (*pp_weblist)->data_size = data_size;
    memset((*pp_weblist)->count, 0, sizeof((*pp_weblist)->count));
    
    // Criar nós e listas..
    if (_create_leaf_node(*pp_weblist, 0) == FAIL) {
        free(*pp_weblist);
        *pp_weblist = NULL;
        return FAIL;
    }

    return SUCCESS;
}

int weblist_destruct(weblist_pp pp_weblist) {
    if (pp_weblist == NULL || *pp_weblist == NULL) return FAIL;

    if (_destroy_leaf_node(*pp_weblist, 0) == FAIL) {
        return FAIL;
    }

    (*pp_weblist) = NULL;

    return SUCCESS;
}

size_t _calc_total_count(size_t *count) {
    return count[0] + count[1] + count[2] + count[3] + count[4] + count[5] + count[6] + count[7];
}

int _add_data(weblist_p root, void *data, int level) {
    int mod = _calc_total_count(root->count) % _calc_number_of_nodes_by_level(level);

    if (root->depth == level) {
        if (iEnd(root->leafs[mod].list, data) == FAIL)
            return FAIL;

    } else {
        if (_add_data(root->leafs[mod].node, data, level + 1) == FAIL)
            return FAIL;
        
    }

    root->count[mod]++;

    return SUCCESS;
}

int weblist_add_data(weblist_p weblist, void *data, compare_fn cmp) {
    if (weblist == NULL || data == NULL) return FAIL; 

    if (_add_data(weblist, data, 0) == FAIL)
        return FAIL;

    return SUCCESS;
}

int weblist_remove_data(weblist_p weblist, void *data, compare_fn cmp) {
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
