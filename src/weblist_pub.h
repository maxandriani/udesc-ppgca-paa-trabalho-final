#ifndef WEBLIST_PUB_H
#define WEBLIST_PUB_H

#include <stdlib.h>
#include "DDLL_pub.h"

// TODO
# define SUCCESS 0
# define FAIL -1

/**
 * Uma definição de uma função genérica que deve receber dois dados abstratos e compara-los.
 * 
 * @param a valor base a ser comparado.
 * @param b valor a comparar com.
 * @return `0` caso `a` e `b` sejam iguais.
 * @return `-1` Inteiro negativo caso `a` seja menor do que `b`.
 * @return `1` Inteiro positivo caso `a` seja maior do que `b`.
 */
typedef int (*compare_fn)(void *a, void *b);

/**
 * Uma definição de uma função genérica que deve executar uma operação com o dado recebido por referência.
 * 
 * @param a valor a ser processado.
 * @param key a chave correspondente a região na qual o dado está armazenado. Não é o índice do valor.
 * @returns `0` se o valor foi processado corretamente.
 * @returns `-1` caso contrário, e irá interromper o laço.
 */
typedef int (*process_fn)(void *a, int key);

/**
 * Descritor da estrutura de dados.
 * 
 * `weblist_t` se refere a referência direta da estrutura.
 * `weblist_p` é um ponteiro direto.
 * `weblist_pp` é um ponteiro indireto.
 */
typedef struct weblist_t *weblist_p, **weblist_pp;

/**
 * Constrói uma Weblist com base em um ponteiro para um descritor.
 * 
 * @param pp_weblist referência para um ponteiro de um descritor `&p_weblist`.
 * @returns 0 se a estrutura foi criada e inicializada corretamente,
 * @returns -1 caso contrário.
 */
int weblist_create(weblist_pp pp_weblist, size_t depth, size_t data_size);

/**
 * Desaloca um descritor e todas as suas dependências.
 * 
 * @param pp_weblist referência para um ponteiro de um descritor `&p_weblist`
 * @param cmp referência de uma função de comparação dos dados.
 * @return 0 se a estrutura e suas dependências foram destruídas corretamente,
 * @return -1 caso contrário. 
 */
int weblist_destruct(weblist_pp pp_weblist); // destruir a estrutura ;


/**
 * Adiciona um novo registro na coleção.
 * 
 * @param weblist referência do descritor da estrutura.
 * @param data dado a ser inserido na estrutura.
 * @param cmp função de comparação para ordenação dos dados.
 * @returns `0` se o dado for adicionado corretamente.
 * @returns `-1` caso contrário.
 */
int weblist_add_data(weblist_p weblist, void *data, compare_fn compare);

/**
 * Remove um registro na coleção.
 * 
 * @param weblist referência do descritor da estrutura.
 * @param cmp função de comparação dos dados.
 * @returns `0` se o dado foi removido corretamente.
 * @returns `1` caso contrário.
 */
int weblist_remove_data(weblist_p weblist, void *data, compare_fn compare);

/**
 * Verificar se um dado existe na coleção.
 * 
 * @param weblist referência do descritor da estrutura.
 * @param value dado a ser pesquisado.
 * @param cmp função de comparação dos dados.
 * @returns `0` se o dado estiver presente na coleção.
 * @returns `-1` caso contrário.
 */
int weblist_search_data(weblist_p weblist, void *data, compare_fn cmp);

/**
 * Percorre a coleção item a item.
 * 
 * @param weblist referência do descritor da estrutura.
 * @param cb função a ser invocada para cada registro presente na estrutura.
 * @returns `0` se a coleção foi percorrida integralmente.
 * @returns `-1` caso contrário.
 */
int weblist_walk_data(weblist_p weblist, process_fn cb);


/**
 * Realiza uma cópia da DDLL correspondente a chave provida.
 * 
 * @param weblist referência do descritor da estrutura.
 * @param key Índice da chave associada a sub coleção desejada.
 * @param list referência ao ponteiro do descritor da lista desejada.
 * @returns `0` caso a chave exista, e a lista correspondente seja corretamente copiada.
 * @returns `-1` caso contrário.
 */
int weblist_copy_list_by_key(weblist_p weblist, int key, ppDDLL list);

/**
 * Substitui os valores da lista correspondente a chave informada pelos valores da lista recebida por parâmetro.
 * 
 * @param weblist referência do descritor da estrutura.
 * @param key Índice da chave associada a sub coleção desejada.
 * @param list referência do descritor da lista (somente leitura) a ser copiada.
 * @returns `0` caso a chave exista, e a lista seja corretamente substituída.
 * @returns `-1` caso contrário.
 */
int weblist_replace_list_by_key(weblist_p weblist, int key, pDDLL list);

/**
 * Remove uma subcoleção da estrutura correspondente a chave informada e copia os dados para uma lista de saída.
 * 
 * @param weblist referência do descritor da estrutura.
 * @param key Índice da chave associada a sub coleção desejada.
 * @param list referência ao ponteiro do descritor da lista a receber os dados removidos.
 * @returns `0` caso a chave exista, e a lista seja copiada e removida corretamente.
 * @returns `-1` caso contrário.
 */
int weblist_remove_list_by_key(weblist_p weblist, int key, ppDDLL list);

/**
 * Cria uma lista vazia na chave de referência.
 * 
 * @param weblist referência do descritor da estrutura.
 * @param key Índice da chave associada a sub coleção desejada.
 * @returns `0` caso a chave ainda não possua uma lista, e uma nova lista seja criada e inicializada.
 * @returns `-1` caso contrário.
 */
int weblist_create_list_by_key(weblist_p weblist, int key);


/**
 * Recupera o total de elementos presentes em uma chave.
 * 
 * @param weblist referência do descritor da estrutura.
 * @param key Índice da chave associada a sub coleção desejada.
 * @param count Retorno da operação.
 * @returns `0` Caso a chave exista e a soma foi realizada corretamente.
 * @returns `-1` Caso contrário.
 */
int weblist_count_by_key(weblist_p weblist, int key, int *count);

/**
 * Recupera o total de chaves presentes da estrutura.
 * 
 * @param weblist referência do descritor da estrutura.
 * @param count Retorno da operação.
 * @returns `0` Caso a operação seja realizada corretamente.
 * @returns `-1` Caso contrário.
 */
int weblist_total_of_keys(weblist_p weblist, int *count);

/**
 * Retorna o total de elementos armazenados na coleção e suas sub coleções.
 * 
 * @param weblist referência do descritor da estrutura.
 * @param count Retorno da operação.
 * @returns `0` Caso a operação seja realizada corretamente.
 * @returns `-1` Caso contrário.
 */
int weblist_count(weblist_p weblist, int *count);

/**
 * Retorna uma lista com todas as chaves contidas na coleção.
 * 
 * @param weblist referência do descritor da estrutura.
 * @param keys referência do descritor da lista que armazenará as chaves da coleção.
 * @returns `0` Caso a operação seja realizada corretamente.
 * @returns `-1` Caso contrário.
 */
int weblist_get_keys(weblist_p weblist, ppDDLL keys);

/**
 * Retorna se a estrutura está balanceada ou não.
 * 
 * @param weblist referência do descritor da estrutura.
 * @returns `0` se a estrutura estiver balanceada.
 * @returns `-1` caso contrário.
 */
int weblist_is_balanced(weblist_p weblist);

#endif