#include "weblist_pri.h"

int _create_leaf_node(list_pp list, size_t data_size) {
    *list = malloc(sizeof(list_p));
    if (*list == NULL) return FAIL;
    (*list)->count = 0;
    (*list)->data = NULL;
    (*list)->data_size = data_size;
    (*list)->next = NULL;
    (*list)->prev = NULL;

    return cDDLL(&(*list)->data, data_size);
}

int _create_node(weblist_pp root, list_pp last, size_t level, size_t depth, size_t data_size) {
    (*root) = malloc(sizeof(weblist_t));
    
    if (*root == NULL) return FAIL;

    (*root)->level = level;
    memset((*root)->boundaries, 0, data_size * 8);
    
    if (level == depth) {
        // Cria as listas
        for (size_t i = 0; i < 8; i++) {
            if (_create_leaf_node(&((*root)->leafs[i].list), data_size) == FAIL) return FAIL;
            
            if (last != NULL)
                (*last)->next = (*root)->leafs[i].list;
            
            (*root)->leafs[i].list->prev = *last;
            (*last) = (*root)->leafs[i].list;
        }
    } else {
        // Cria um nó
        for (size_t i = 0; i < 8; i++) {
            if (_create_node(&((*root)->leafs[i].node), last, level + 1, depth, data_size) == FAIL) return FAIL;
        }
    }

    return SUCCESS;
}

int _destroy_leaf_node(list_pp list) {
    if ((*list)->data != NULL) {
        if (dDDLL(&(*list)->data) == FAIL)
            return FAIL;
    }

    free(*list);
    (*list) = NULL;

    return SUCCESS;
}

int _destroy_node(weblist_pp root, size_t level) {
    if ((*root)->level == level) {
        for (size_t i = 0; i < 8; i++) {
            if (_destroy_leaf_node(&(*root)->leafs[i].list) == FAIL)
                return FAIL;
        }
    } else {
        for (size_t i = 0; i < 8; i++) {
            if (_destroy_node(&(*root)->leafs[i].node, (level + 1)) == FAIL)
                return FAIL;
        }
    }

    free(*root);
    (*root) = NULL;

    return SUCCESS;
}

// int _calc_number_of_nodes_by_level(int level) {
//     return pow(8, (level + 1));
// }

int weblist_create(weblist_pp pp_weblist, size_t depth, size_t data_size) {
    if (pp_weblist == NULL || *pp_weblist != NULL || data_size == 0) return FAIL;

    list_p last = NULL;
    return _create_node(pp_weblist, &last, 0, depth, data_size);
}

int weblist_destruct(weblist_pp pp_weblist) {
    if (pp_weblist == NULL || *pp_weblist == NULL) return FAIL;

    return _destroy_node(pp_weblist, 0);
}

int weblist_add_data(weblist_p weblist, void *data, compare_fn cmp) {
    if (weblist == NULL || data == NULL) return FAIL; 

    return SUCCESS;
}

int weblist_remove_data(weblist_p weblist, void *data, compare_fn cmp) {
    return FAIL;
}

int weblist_search_data(weblist_p weblist, void *data, compare_fn cmp) {
    return FAIL;
}

int _walk_ddll_list(list_p list, process_fn cb) {
    pDDLL tmp = NULL;
    void *element = NULL;
    int result = SUCCESS;
    
    cDDLL(&tmp, list->data_size);
    for (size_t i = 0; i < list->count; i++) {
        rBegin(list->data, element);
        iEnd(tmp, element);
        result |= cb(element, list->key);
    }
    dDDLL(&list->data);
    list->data = tmp;

    return result;
}

int _walk_node(weblist_p root, size_t level, process_fn cb) {
    if (root->level == level) {
        for (size_t i = 0; i < 8; i++) {
            if (_walk_ddll_list(root->leafs[i].list, cb) != SUCCESS)
                return FAIL;
        }
    } else {
        for (size_t i = 0; i < 8; i++) {
            if (_walk_node(root->leafs[i].node, (level + 1), cb) != SUCCESS)
                return FAIL;
        } 
    }

    return SUCCESS;
}

int weblist_walk_data(weblist_p weblist, process_fn cb) {
    if (weblist == NULL || cb == NULL)
        return FAIL;

    return _walk_node(weblist, 0, cb);
}

int _get_list_by_key(weblist_p root, list_pp list, size_t level, int key) {
    if (root->level == level) {
        for (size_t i = 0; i < 8; i++) {
            if (root->leafs[i].list->key == key) {
                *list = root->leafs[i].list;
                return SUCCESS;
            }
        }
    } else {
        for (size_t i = 0; i < 8; i++) {
            if (_get_list_by_key(root->leafs[i].node, list, (level + 1), key) == SUCCESS)
                return SUCCESS;
        }
    }

    return FAIL;
}

int weblist_copy_list_by_key(weblist_p weblist, int key, ppDDLL list) {
    if (weblist == NULL || key < 0 || list == NULL || *list != NULL)
        return FAIL;

    list_p local_list = NULL;
    pDDLL tmp = NULL;
    void *element = NULL;

    if (_get_list_by_key(weblist, &local_list, 0, key) == FAIL)
        return FAIL;
    
    cDDLL(&tmp, local_list->data_size);
    cDDLL(list, local_list->data_size);
    
    for (size_t i = 0; i < local_list->count; i++) {
        rBegin(local_list->data, element);
        iEnd(tmp, element);
        iEnd(*list, element);
    }

    dDDLL(&local_list->data);
    local_list->data = tmp;

    return SUCCESS;
}

int weblist_replace_list_by_key(weblist_p weblist, int key, pDDLL list, compare_fn cmp) {
    if (weblist == NULL || key < 0 || list == NULL)
        return FAIL;

    list_p local_list = NULL;
    size_t idx = 0;
    void *element = NULL;

    if (_get_list_by_key(weblist, &local_list, 0, key) == FAIL)
        return FAIL;

    cleanDDLL(local_list->data);

    while (sPosition(list, idx, element) != FAIL) {
        weblist_add_data(weblist, element, cmp);
        idx++;
    }
    
    return SUCCESS;
}

int weblist_remove_list_by_key(weblist_p weblist, int key, ppDDLL list) {
    if (weblist == NULL || key < 0 || list == NULL)
        return FAIL;

    list_p local_list = NULL;

    if (_get_list_by_key(weblist, &local_list, 0, key) == FAIL)
        return FAIL;

    return cleanDDLL(local_list->data);
}

int weblist_count_by_key(weblist_p weblist, int key, int *count) {
    if (weblist == NULL || key < 0 || count == NULL)
        return FAIL;

    list_p local_list = NULL;

    if (_get_list_by_key(weblist, &local_list, 0, key) == FAIL)
        return FAIL;

    *count = local_list->count;

    return SUCCESS;
}

int weblist_total_of_keys(weblist_p weblist, int *count) {
    if (weblist == NULL || count == NULL)
        return FAIL;

    *count = pow(8, weblist->level);

    return SUCCESS;
}

int weblist_count(weblist_p weblist, int *count) {
    if (weblist == NULL || count == NULL)
        return FAIL;

    list_p local_list = NULL;
    *count = 0;

    if (_get_list_by_key(weblist, &local_list, 0, 0) == FAIL)
        return FAIL;

    while (local_list != NULL) {
        local_list = local_list->next;
        (*count) += local_list->count;
    }
    
    return SUCCESS;
}

int weblist_get_keys(weblist_p weblist, ppDDLL keys) {
    if (weblist == NULL || keys == NULL || *keys != NULL)
        return FAIL;

    int total_of_keys = 0;

    if (weblist_total_of_keys(weblist, &total_of_keys) == FAIL)
        return FAIL;

    cDDLL(keys, sizeof(int));
    for (size_t i = 0; i < total_of_keys; i++) {
        iEnd(*keys, &i);
    }
    
    return SUCCESS;
}

int weblist_is_balanced(weblist_p weblist) {
    return SUCCESS;
}
