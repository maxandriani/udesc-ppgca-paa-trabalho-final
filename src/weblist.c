#include "weblist_pri.h"

int _is_leaf_node(weblist_p root) {
    return root->depth == root->level;
}

int _search_by_value(list_p list, void *value, compare_fn cmp) {
    void *element = malloc(list->data_size);
    int idx = -1;

    for (size_t i = 0; i < list->count; i++) {
        rBegin(list->data, element);
        iEnd(list->data, element);
        if (cmp(element, value) == 0) {
            idx = i;
            break;
        }
    }

    free(element);

    return idx;
}

int _search_and_delete_by_value(list_p list, void *value, compare_fn cmp) {
    int idx = _search_by_value(list, value, cmp);
    void *element = malloc(list->data_size);
    if (idx > -1)
        rEnd(list->data, element);
    free(element);
    return idx;
}

int _find_min_value(list_p list, void *value, compare_fn cmp) {
    void *current = malloc(list->data_size);
    int idx = -1;

    memset(value, 0, list->data_size);
    memset(current, 0, list->data_size);

    for (size_t i = 0; i < list->count; i++) {
        rBegin(list->data, current);
        iEnd(list->data, current);
        
        if (i == 0 || cmp(current, value) < 0) {
            memcpy(value, current, list->data_size);
            idx = i;
            break;
        }
    }

    free(current);

    return idx;
}

int _find_max_value(list_p list, void *value, compare_fn cmp) {
    void *current = malloc(list->data_size);
    int idx = -1;

    memset(value, 0, list->data_size);
    memset(current, 0, list->data_size);

    for (size_t i = 0; i < list->count; i++) {
        rBegin(list->data, current);
        iEnd(list->data, current);
        
        if (i == 0 || cmp(current, value) > 0) {
            memcpy(value, current, list->data_size);
            idx = i;
            break;
        }
    }

    free(current);

    return idx;
}

int _create_leaf_node(list_pp list, size_t data_size) {
    *list = malloc(sizeof(list_p));
    if (*list == NULL) return FAIL;
    (*list)->count = 0;
    (*list)->data = NULL;
    (*list)->data_size = data_size;
    (*list)->next = NULL;
    (*list)->prev = NULL;
    (*list)->root = NULL;
    (*list)->key = 0;

    return cDDLL(&(*list)->data, data_size);
}

int _create_node(weblist_pp root, list_pp last, size_t level, size_t depth, size_t data_size) {
    (*root) = malloc(sizeof(weblist_t));
    
    if (*root == NULL) return FAIL;

    (*root)->level = level;
    (*root)->depth = depth;
    memset((*root)->boundaries, 0, data_size * 8);
    
    if (_is_leaf_node(*root)) {
        // Cria as listas
        for (size_t i = 0; i < 8; i++) {
            if (_create_leaf_node(&((*root)->leafs[i].list), data_size) == FAIL) return FAIL;
            
            if (last != NULL) {
                (*root)->leafs[i].list->key = (*last)->key + 1;
                (*last)->next = (*root)->leafs[i].list;
            }
            
            (*root)->leafs[i].list->prev = *last;
            (*last) = (*root)->leafs[i].list;
            (*last)->root = (*root);
        }
    } else {
        // Cria um nó
        for (size_t i = 0; i < 8; i++) {
            if (_create_node(&((*root)->leafs[i].node), last, level + 1, depth, data_size) == FAIL) return FAIL;
            (*root)->leafs[i].node->root = (*root);
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

int _destroy_node(weblist_pp root) {
    if (_is_leaf_node(*root)) {
        for (size_t i = 0; i < 8; i++) {
            if (_destroy_leaf_node(&(*root)->leafs[i].list) == FAIL)
                return FAIL;
        }
    } else {
        for (size_t i = 0; i < 8; i++) {
            if (_destroy_node(&(*root)->leafs[i].node) == FAIL)
                return FAIL;
        }
    }

    free(*root);
    (*root) = NULL;

    return SUCCESS;
}

int weblist_create(weblist_pp pp_weblist, size_t depth, size_t data_size) {
    if (pp_weblist == NULL || *pp_weblist != NULL || data_size == 0) return FAIL;

    list_p last = NULL;
    return _create_node(pp_weblist, &last, 0, depth, data_size);
}

int weblist_destruct(weblist_pp pp_weblist) {
    if (pp_weblist == NULL || *pp_weblist == NULL) return FAIL;

    return _destroy_node(pp_weblist);
}

void _balance(list_p list, size_t depth, compare_fn cmp) {

}

size_t _calc_insert_idx(weblist_p root, void *data, compare_fn cmp) {
    size_t idx = 0;
    while (idx < 8 && root->boundaries[idx] != NULL && cmp(root->boundaries[idx], data) < 0)
        idx++;
    return idx;
}

int _add_data(weblist_p root, void *data, compare_fn cmp) {
    if (_is_leaf_node(root)) {
        size_t idx = _calc_insert_idx(root, data, cmp);
        list_p leaf = root->leafs[idx].list;

        leaf->count++;
        iEnd(leaf->data, data);
        _balance(leaf, root->depth, cmp);

        return SUCCESS;
    } else {
        for (size_t i = 0; i < 7; i++) {
            if (root->boundaries[i] == NULL || cmp(&root->boundaries[i], data) < 0) {
                // insert left
                return _add_data(root->leafs[i].node, data, cmp);
            }
        }

        return _add_data(root->leafs[7].node, data, cmp);
    }
}

int weblist_add_data(weblist_p weblist, void *data, compare_fn cmp) {
    if (weblist == NULL || data == NULL) return FAIL;

    return _add_data(weblist, data, cmp);
}

int _remove_data(weblist_p root, void *data, compare_fn cmp) {
    if (_is_leaf_node(root)) {
        size_t idx = _calc_insert_idx(root, data, cmp);
        list_p leaf = root->leafs[idx].list;
        int found_at_idx = _search_and_delete_by_value(leaf, data, cmp);
        
        if (found_at_idx == -1) return FAIL;

        _balance(leaf, root->depth, cmp);

        return SUCCESS;
    } else {
        for (size_t i = 0; i < 7; i++) {
            if (root->boundaries[i] == NULL || cmp(&root->boundaries[i], data) < 0) {
                // insert left
                return _add_data(root->leafs[i].node, data, cmp);
            }
        }

        return _add_data(root->leafs[7].node, data, cmp);
    }
}

int weblist_remove_data(weblist_p weblist, void *data, compare_fn cmp) {
    if (weblist == NULL || data == NULL || cmp == NULL)
        return FAIL;

    return _remove_data(weblist, data, cmp);
}

int _search_data(weblist_p root, void *data, compare_fn cmp) {
    if (_is_leaf_node(root)) {
        size_t idx = _calc_insert_idx(root, data, cmp);
        list_p leaf = root->leafs[idx].list;
        int found_at_idx = _search_by_value(leaf, data, cmp);
        
        if (found_at_idx > -1) return SUCCESS;

        return FAIL;
    } else {
        for (size_t i = 0; i < 7; i++) {
            if (root->boundaries[i] == NULL || cmp(&root->boundaries[i], data) < 0)
                return _add_data(root->leafs[i].node, data, cmp);
        }

        return _add_data(root->leafs[7].node, data, cmp);
    }
}

int weblist_search_data(weblist_p weblist, void *data, compare_fn cmp) {
    if (weblist == NULL || data == NULL || cmp == NULL)
        return FAIL;
    
    return _search_data(weblist, data, cmp);
}

int _walk_ddll_list(list_p list, process_fn cb) {
    void *element = malloc(list->data_size);
    int result = SUCCESS;
    
    for (size_t i = 0; i < list->count; i++) {
        rBegin(list->data, element);
        iEnd(list->data, element);
        result |= cb(element, list->key);
    }

    free(element);

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

int _get_list_by_key(weblist_p root, list_pp list, int key) {
    int divisor = key / pow(8, (root->depth - root->level));
    int idx = divisor % 8;
    if (_is_leaf_node(root)) {
        *list = root->leafs[idx].list;
        return SUCCESS;
    } else {
        return _get_list_by_key(root->leafs[idx].node, list, key);
    }

    return FAIL;
}

int weblist_copy_list_by_key(weblist_p weblist, int key, ppDDLL list) {
    if (weblist == NULL || key < 0 || list == NULL || *list != NULL)
        return FAIL;

    list_p local_list = NULL;
    pDDLL tmp = NULL;
    void *element = NULL;

    if (_get_list_by_key(weblist, &local_list, key) == FAIL)
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

    if (_get_list_by_key(weblist, &local_list, key) == FAIL)
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

    if (_get_list_by_key(weblist, &local_list, key) == FAIL)
        return FAIL;

    return cleanDDLL(local_list->data);
}

int weblist_count_by_key(weblist_p weblist, int key, int *count) {
    if (weblist == NULL || key < 0 || count == NULL)
        return FAIL;

    list_p local_list = NULL;

    if (_get_list_by_key(weblist, &local_list, key) == FAIL)
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

    if (_get_list_by_key(weblist, &local_list, 0) == FAIL)
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

list_p _get_leaftish_leaf(weblist_p root) {
    while (root != NULL && !_is_leaf_node(root))
        root = root->leafs[0].node;

    if (root != NULL)
        return root->leafs[0].list;

    return NULL;
}

int weblist_is_balanced(weblist_p weblist) {
    if (weblist == NULL) return FAIL;

    list_p leaf = _get_leaftish_leaf(weblist);

    while (leaf != NULL && leaf->next != NULL) {
        if (abs((int) leaf->count - (int)leaf->next->count) > 1) {
            return FAIL;
        }
        leaf = leaf->next;
    }

    return SUCCESS;
}
