#include <stdlib.h>
#include <stdio.h>
#include "main.h"

void print_result(int result, const char* msg) {
    if (result == FAIL) {
        printf("FAIL: %s\n", msg);
    } else {
        printf("SUCCESS: %s\n", msg);
    }
}

int main(int argc, char *argv[]) {
    should_not_create_a_weblist_from_null_pointer();
    should_only_create_a_weblist_from_null_ref();
    should_only_create_a_weblist_from_null_ref();
    should_not_add_items_on_weblist_from_null_pointer();
    should_create_a_weblist_of_level_3();
    should_add_itens_on_a_weblist_of_level_2_and_keep_balance();
    return 0;
}

void should_not_create_a_weblist_from_null_pointer() {
    int result = SUCCESS;
    weblist_p weblist = NULL;
    result |= weblist_create(NULL, 3, 1) == SUCCESS;
    result |= weblist_create(&weblist, 3, 0) == SUCCESS;

    print_result(result, __func__);
}

void should_only_create_a_weblist_from_null_ref() {
    weblist_p weblist1 = NULL;
    weblist_p weblist2 = (weblist_p) 0x873;

    int result = SUCCESS;
    result |= weblist_create(&weblist1, 3, 4) == FAIL;
    result |= weblist_create(&weblist2, 3, 4) == SUCCESS;

    weblist_destruct(&weblist1);
    print_result(result, __func__);
}

void should_not_add_items_on_weblist_from_null_pointer() {
    int result = SUCCESS;
    result |= weblist_add_data(NULL, (void *) 0x8773) == SUCCESS;
    result |= weblist_add_data((weblist_p) 0x988, NULL) == SUCCESS;

    print_result(result, __func__);
}

void should_create_a_weblist_of_level_3() {
    weblist_p weblist = NULL;
    int result = SUCCESS;

    result |= weblist_create(&weblist, 3, 4) == FAIL;

    weblist_destruct(&weblist);

    print_result(result, __func__);
}

void should_add_itens_on_a_weblist_of_level_2_and_keep_balance() {
    int elements[64];
    weblist_p weblist = NULL;
    int result = SUCCESS;

    for (size_t i = 0; i < 64; i++){
        elements[i] = i;
    }
    
    result |= weblist_create(&weblist, 2, 4) == FAIL;

    for (size_t i = 0; i < 64; i++){
        result |= weblist_add_data(weblist, &elements[i]) == FAIL;
    }

    // REVISAR PRA VERIFICAR COM LISTA

    weblist_destruct(&weblist);

    print_result(result, __func__);
}