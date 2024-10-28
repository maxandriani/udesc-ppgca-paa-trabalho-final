#include "DDLL_pri.h"

int _ddll_create_node(pDDLL p_ddll, pp_node_t pp_node, p_node_t prev, p_node_t next, void * value) {
    if (pp_node == NULL || *pp_node != NULL || value == NULL)
        return FAIL;

    *pp_node = malloc(sizeof(node_t));
    
    if (*pp_node == NULL)
        return FAIL;

    (*pp_node)->value = malloc(p_ddll->data_size);

    if ((*pp_node)->value == NULL)
        return FAIL;

    memcpy((*pp_node)->value, value, p_ddll->data_size);
    (*pp_node)->next = next;
    (*pp_node)->prev = prev;

    if (next != NULL) {
        next->prev = *pp_node;
    } else {
        p_ddll->tail = *pp_node;
    }

    if (prev != NULL) {
        prev->next = *pp_node;
    } else {
        p_ddll->head = *pp_node;
    }

    return SUCCESS;
}

int _ddll_remove_node(pDDLL p_ddll, p_node_t p_node, void * value) {
    if (p_node == NULL)
        return FAIL;

    if (value != NULL)
        memcpy(value, p_node->value, p_ddll->data_size);
    
    if (p_node->prev != NULL) {
        p_node->prev->next = p_node->next;
    } else {
        p_ddll->head = p_node->next;
    }

    if (p_node->next != NULL) {
        p_node->next->prev = p_node->prev;
    } else {
        p_ddll->tail = p_node->prev;
    }

    free(p_node->value);
    free(p_node);

    return SUCCESS;
}

int cDDLL(ppDDLL pp, int sizedata) {
    if (pp == NULL || *pp != NULL)
        return FAIL;

    if (sizedata < 1)
        return FAIL;

    (*pp) = malloc(sizeof(DDLL));

    if (*pp == NULL)
        return FAIL;

    (*pp)->head = NULL;
    (*pp)->tail = NULL;
    (*pp)->data_size = sizedata;
    (*pp)->length = 0;

    return SUCCESS;
}

int dDDLL(ppDDLL pp) {
    if (pp == NULL || (*pp) == NULL)
        return FAIL;

    cleanDDLL(*pp);
    free(*pp);
    *pp = NULL;

    return SUCCESS;
}

int cleanDDLL(pDDLL p) {
    if (p == NULL)
        return FAIL;

    while (p->head != NULL) {
        if (_ddll_remove_node(p, p->head, NULL) == FAIL)
            break;
    }

    p->length = 0;
    p->tail = NULL;

    return SUCCESS;
}

int iBegin(pDDLL p, void *element) {
    p_node_t node = NULL;

    if (p == NULL || element == NULL)
        return FAIL;

    if (_ddll_create_node(p, &node, NULL, p->head, element) == FAIL)
        return FAIL;

    p->length++;

    return SUCCESS;
}

int iEnd(pDDLL p, void *element) {
    p_node_t node = NULL;

    if (p == NULL || element == NULL)
        return FAIL;
    
    if (_ddll_create_node(p, &node, p->tail, NULL, element) == FAIL)
        return FAIL;

    p->length++;

    return SUCCESS;
}

int iPosition(pDDLL p, int N, void *element) {
    p_node_t next = NULL,
             prev = NULL,
             node = NULL;

    if (p == NULL || element == NULL || N < 0 || N >= p->length)
        return FAIL;

    next = p->head;

    for (int i = 0; i < N; i++) {
        next = next->next;
    }
    
    if (next != NULL) {
        prev = next->prev;
    }

    if (_ddll_create_node(p, &node, prev, next, element) == FAIL)
        return FAIL;

    p->length++;

    return SUCCESS;
}

int rBegin(pDDLL p, void *element) {
    if (p == NULL || element == NULL || empty(p) == 0)
        return FAIL;

    if (_ddll_remove_node(p, p->head, element) == FAIL)
        return FAIL;

    p->length--;

    return SUCCESS;
}

int rEnd(pDDLL p, void *element) {
    if (p == NULL || element == NULL || empty(p) == 0)
        return FAIL;
    
    if (_ddll_remove_node(p, p->tail, element) == FAIL)
        return FAIL;

    p->length--;

    return SUCCESS;
}

int rPosition(pDDLL p, int N, void *element) {
    p_node_t current = NULL;
    
    if (p == NULL || element == NULL || N < 0 || N >= p->length)
        return FAIL;
    
    current = p->head;
    for (int i = 0; i < N; i++) {
        current = current->next;
    }
    
    if (_ddll_remove_node(p, current, element) == FAIL)
        return FAIL;

    p->length--;

    return SUCCESS;
}

int sBegin(pDDLL p, void *element) {
    if (p == NULL || element == NULL || empty(p) == 0)
        return FAIL;
    
    memcpy(element, p->head->value, p->data_size);

    return SUCCESS;
}

int sEnd(pDDLL p, void *element) {
    if (p == NULL || element == NULL || empty(p) == 0L)
        return FAIL;
    
    memcpy(element, p->tail->value, p->data_size);

    return SUCCESS;
}

int sPosition(pDDLL p, int N, void *element) {
    p_node_t current = NULL;
    
    if (p == NULL || element == NULL || N < 0 || N >= p->length)
        return FAIL;
    
    current = p->head;
    for (int i = 0; i < N; i++) {
        current = current->next;
    }
    
    memcpy(element, current->value, p->data_size);

    return SUCCESS;
}

// Funções adicionais
int empty(pDDLL p) {
    return (p != NULL && p->length > 0);
}

int countElements(pDDLL p) {
    if (p == NULL) return 0;

    return p->length;
}

int sValue(pDDLL p, int *position, void * condition, int (*predicate)(void * a, void * b)) {
    p_node_t current = NULL;
    
    if (p == NULL || position == NULL || condition == NULL || predicate == NULL)
        return FAIL;

    current = p->head;
    *position = -1;

    for (int i = 0; i < p->length; i++) {
        if (predicate(condition, current->value)) {
            *position = i;
            break;
        }
        current = current->next;
    }

    return SUCCESS;
}