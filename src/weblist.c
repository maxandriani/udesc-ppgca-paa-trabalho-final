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

int _remove_list_data_by_value(list_p list, void *value, compare_fn cmp) {
    void *current;

    if (list->count == 0)
        return FAIL;

    current = malloc(list->data_size);

    for (size_t i = 0; i < list->count; i++) {
        rBegin(list->data, current);
        
        if (cmp(current, value) == 0)
            break;

        iEnd(list->data, current);
    }

    free(current);

    return SUCCESS;
}

int _find_min_value(list_p list, void *value, compare_fn cmp) {
    void *current;

    if (list->count == 0)
        return FAIL;

    rBegin(list->data, value);
    iEnd(list->data, value);

    current = malloc(list->data_size);

    for (size_t i = 1; i < list->count; i++) {
        rBegin(list->data, current);
        iEnd(list->data, current);
        
        if (cmp(current, value) < 0) {
            memcpy(value, current, list->data_size);
        }
    }

    free(current);

    return SUCCESS;
}

int _find_max_value(list_p list, void *value, compare_fn cmp) {
    void *current;

    if (list->count == 0)
        return FAIL;

    rBegin(list->data, value);
    iEnd(list->data, value);

    current = malloc(list->data_size);

    for (size_t i = 1; i < list->count; i++) {
        rBegin(list->data, current);
        iEnd(list->data, current);
        
        if (cmp(current, value) > 0) {
            memcpy(value, current, list->data_size);
        }
    }

    free(current);

    return SUCCESS;
}

int _create_leaf_node(list_pp list, size_t data_size) {
    *list = malloc(sizeof(list_t));
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
    (*root)->data_size = data_size;
    
    for (size_t i = 0; i < 8; i++) {
        if (_is_leaf_node(*root)) {
        // Cria as listas
        
            if (_create_leaf_node(&((*root)->leafs[i].list), data_size) == FAIL) return FAIL;
            
            if (*last != NULL) {
                (*root)->leafs[i].list->key = (*last)->key + 1;
                (*last)->next = (*root)->leafs[i].list;
            }
            
            (*root)->leafs[i].list->prev = *last;
            (*last) = (*root)->leafs[i].list;
            (*last)->root = (*root);
        } else {
        // Cria um nó
            if (_create_node(&((*root)->leafs[i].node), last, level + 1, depth, data_size) == FAIL) return FAIL;
            (*root)->leafs[i].node->root = (*root);
        }

        (*root)->boundaries[i] = NULL;
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
    for (size_t i = 0; i < 8; i++) {
        if (_is_leaf_node(*root)) {
            if (_destroy_leaf_node(&(*root)->leafs[i].list) == FAIL)
                return FAIL;
        } else {
            if (_destroy_node(&(*root)->leafs[i].node) == FAIL)
                return FAIL;
        }
        if ((*root)->boundaries[i] != NULL)
            free((*root)->boundaries[i]);
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

void _update_index(weblist_p node, int key, void *element) {
    if (node == NULL || element == NULL) return;

    int divisor = key / pow(8, (node->depth - node->level));
    int idx = divisor % 8;

    if (node->boundaries[idx] == NULL)
        node->boundaries[idx] = malloc(node->data_size);

    // memcpy(node->boundaries[idx], element, node->data_size);

    if (idx == 0 && node->root != NULL)
        _update_index(node->root, key, element);
}

void _shift_left(list_p list, compare_fn cmp) {
    list_p current = list->next;
    void *element;
    
    while (current != NULL && current->count == 0)
        current = current->next;

    if (current == NULL) return;

    element = malloc(list->data_size);

    _find_min_value(current, element, cmp);
    iEnd(list->data, element);
    list->count++;
    _remove_list_data_by_value(current, element, cmp);
    current->count--;

    free(element);
}

void _shift_right(list_p list, compare_fn cmp) {
    list_p current = list->next;
    void *element = NULL;
    
    if (list->next == NULL) return;

    element = malloc(list->data_size);

    _find_max_value(list, element, cmp);
    iEnd(current->data, element);
    current->count++;
    _remove_list_data_by_value(list, element, cmp);
    list->count--;
    
    free(element);
}

void _balance(weblist_p root, compare_fn cmp) {
    list_p head = _get_leaftish_leaf(root);
    list_p current = head;
    int count = 0;
    size_t max_count = 0;
    int total_of_keys;
    int idx_flip;
    int should_rebuild = 0;
    void *element = NULL;

    weblist_total_of_keys(root, &total_of_keys);
    weblist_count(root, &count);
    
    max_count = count / total_of_keys;
    idx_flip = count % total_of_keys;

    while (current != NULL) {
        // metade do count + 1 --- resto
        if (current->key < idx_flip) {
            while (current->count < (max_count + 1)) {
                _shift_left(current, cmp);
                should_rebuild = 1;
            }

            while (current->count > (max_count + 1)) {
                _shift_right(current, cmp);
                should_rebuild = 1;
            }
        } else {
            while (current->count < max_count) {
                _shift_left(current, cmp);
                should_rebuild = 1;
            }

            while (current->count > max_count) {
                _shift_right(current, cmp);
                should_rebuild = 1;
            }
        }

        if (should_rebuild == 1) {
            element = malloc(current->data_size);
            _find_min_value(current, element, cmp);
            _update_index(current->root, current->key, element);
            free(element);
            element = NULL;
        }
        current = current->next;
    }
}

size_t _calc_insert_idx(weblist_p root, void *data, compare_fn cmp) {
    size_t idx = 0;
    while (idx < 7 && root->boundaries[idx + 1] != NULL && cmp(root->boundaries[idx + 1], data) < 0)
        idx++;
    return idx;
}

int _add_data(weblist_p root, void *data, compare_fn cmp) {
    if (_is_leaf_node(root)) {
        size_t idx = _calc_insert_idx(root, data, cmp);
        list_p leaf = root->leafs[idx].list;

        leaf->count++;
        iEnd(leaf->data, data);

        return SUCCESS;
    } else {
        for (size_t i = 1; i < 8; i++) {
            if (root->boundaries[i] == NULL || cmp(data, root->boundaries[i]) < 0) {
                return _add_data(root->leafs[i-1].node, data, cmp);
            }
        }

        return _add_data(root->leafs[7].node, data, cmp);
    }
}

int weblist_add_data(weblist_p weblist, void *data, compare_fn cmp) {
    if (weblist == NULL || data == NULL || cmp == NULL) return FAIL;

    if (_add_data(weblist, data, cmp) == FAIL)
        return FAIL;

    _balance(weblist, cmp);

    return SUCCESS;
}

int _remove_data(weblist_p root, void *data, compare_fn cmp) {
    if (_is_leaf_node(root)) {
        size_t idx = _calc_insert_idx(root, data, cmp);
        list_p leaf = root->leafs[idx].list;
        int found_at_idx = _search_and_delete_by_value(leaf, data, cmp);
        
        if (found_at_idx == -1) return FAIL;

        return SUCCESS;
    } else {
        for (size_t i = 1; i < 8; i++) {
            if (root->boundaries[i] == NULL || cmp(data, root->boundaries[i]) < 0) {
                // insert left
                return _add_data(root->leafs[i-1].node, data, cmp);
            }
        }

        return _add_data(root->leafs[7].node, data, cmp);
    }
}

int weblist_remove_data(weblist_p weblist, void *data, compare_fn cmp) {
    if (weblist == NULL || data == NULL || cmp == NULL)
        return FAIL;

    if (_remove_data(weblist, data, cmp) == FAIL)
        return FAIL;
    
    _balance(weblist, cmp);

    return SUCCESS;
}

int _search_data(weblist_p root, void *data, compare_fn cmp) {
    if (_is_leaf_node(root)) {
        size_t idx = _calc_insert_idx(root, data, cmp);
        list_p leaf = root->leafs[idx].list;
        int found_at_idx = _search_by_value(leaf, data, cmp);
        
        if (found_at_idx > -1) return SUCCESS;

        return FAIL;
    } else {
        for (size_t i = 1; i < 8; i++) {
            if (root->boundaries[i] == NULL || cmp(data, root->boundaries[i]) < 0)
                return _add_data(root->leafs[i-1].node, data, cmp);
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

int _walk_node(weblist_p root, process_fn cb) {
    if (_is_leaf_node(root)) {
        for (size_t i = 0; i < 8; i++) {
            if (_walk_ddll_list(root->leafs[i].list, cb) != SUCCESS)
                return FAIL;
        }
    } else {
        for (size_t i = 0; i < 8; i++) {
            if (_walk_node(root->leafs[i].node, cb) != SUCCESS)
                return FAIL;
        } 
    }

    return SUCCESS;
}

int weblist_walk_data(weblist_p weblist, process_fn cb) {
    if (weblist == NULL || cb == NULL)
        return FAIL;

    return _walk_node(weblist, cb);
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
    void *element = NULL;

    if (_get_list_by_key(weblist, &local_list, key) == FAIL)
        return FAIL;
    
    element = malloc(local_list->data_size);
    cDDLL(list, local_list->data_size);
    
    for (size_t i = 0; i < local_list->count; i++) {
        rBegin(local_list->data, element);
        iEnd(local_list->data, element);
        iEnd(*list, element);
    }

    free(element);

    return SUCCESS;
}

int weblist_replace_list_by_key(weblist_p weblist, int key, pDDLL list, compare_fn cmp) {
    if (weblist == NULL || key < 0 || list == NULL || cmp == NULL)
        return FAIL;

    list_p local_list = NULL;
    void *element = NULL;
    int count = countElements(list);

    if (_get_list_by_key(weblist, &local_list, key) == FAIL)
        return FAIL;

    element = malloc(local_list->data_size);
    cleanDDLL(local_list->data);

    for (size_t i = 0; i < count; i++) {
        rBegin(list, element);
        iEnd(list, element);
        _add_data(weblist, element, cmp);
    }
    
    free(element);

    _balance(weblist, cmp);
    
    return SUCCESS;
}

int weblist_remove_list_by_key(weblist_p weblist, int key, ppDDLL list, compare_fn cmp) {
    if (weblist == NULL || key < 0 || list == NULL || cmp == NULL)
        return FAIL;

    list_p local_list = NULL;
    void *element = NULL;

    if (_get_list_by_key(weblist, &local_list, key) == FAIL)
        return FAIL;

    cDDLL(list, local_list->data_size);
    element = malloc(local_list->data_size);
    for (size_t i = 0; i < local_list->count; i++) {
        rBegin(local_list->data, element);
        iEnd(*list, element);
    }
    free(element);

    local_list->count = 0;
    cleanDDLL(local_list->data);
    _balance(weblist, cmp);

    return SUCCESS;
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

    *count = pow(8, (weblist->depth + 1));

    return SUCCESS;
}

int weblist_count(weblist_p weblist, int *count) {
    if (weblist == NULL || count == NULL)
        return FAIL;

    list_p local_list = _get_leaftish_leaf(weblist);
    *count = 0;

    while (local_list != NULL) {
        (*count) += local_list->count;
        local_list = local_list->next;
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
