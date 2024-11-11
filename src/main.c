#include <stdlib.h>
#include <stdio.h>
#include "main.h"

int * PROCESS_WALK_RESULT = NULL;
size_t PROCESS_WALK_RESULT_IDX = 0;
char LAST_ASSERT_MSG[1024];

void assert(int result, const char* fn_name) {
    if (result == 1) {
        if (strcmp(LAST_ASSERT_MSG, fn_name) != 0) {
            printf("SUCCESS: %s\n", fn_name);
            strcpy(LAST_ASSERT_MSG, fn_name);
        }
    } else {
        printf("FAIL: %s\n", fn_name);
        exit(1);
    }
}

int compare_int(void *a, void *b) {
    return *((int *)a) - *((int *)b);
}

int compare_test_type(void *a, void *b) {
    test_type_t *typed_a = a;
    test_type_t *typed_b = b;

    int level_1 = (typed_a->a - typed_b->a);
    if (level_1 == 0) {
        return (typed_a->b - typed_b->b);
    }
    return level_1;
}

int process_int(void *x, int key) {
    int * int_x = x;
    PROCESS_WALK_RESULT[PROCESS_WALK_RESULT_IDX] = *int_x;
    PROCESS_WALK_RESULT_IDX++;
    return SUCCESS;
}

int process_test_type(void *x, int key) {
    return SUCCESS;
}

void process_int_init(size_t count) {
    if (PROCESS_WALK_RESULT != NULL)
        process_int_destruct();

    PROCESS_WALK_RESULT = malloc(sizeof(int) * count);
    memset(PROCESS_WALK_RESULT, 0, sizeof(int) * count);
}

void process_int_destruct() {
    if (PROCESS_WALK_RESULT != NULL)
        free(PROCESS_WALK_RESULT);
    PROCESS_WALK_RESULT = NULL;
    PROCESS_WALK_RESULT_IDX = 0;
}

int *process_int_walkthrough() {
    return PROCESS_WALK_RESULT;
}

int main(int argc, char *argv[]) {
    should_weblist_create_validate_null_pointers();
    should_weblist_destruct_validate_null_pointers();
    should_weblist_add_data_validate_null_pointers();
    should_weblist_remove_data_validate_null_pointers();
    should_weblist_search_data_validate_null_pointers();
    should_weblist_walk_data_validate_null_pointers();
    should_weblist_copy_list_by_key_validate_null_pointers();
    should_weblist_replace_list_by_key_validate_null_pointers();
    should_weblist_remove_list_by_key_validate_null_pointers();
    should_weblist_count_by_key_validate_null_pointers();
    should_weblist_total_of_keys_validate_null_pointers();
    should_weblist_count_validate_null_pointers();
    should_weblist_get_keys_validate_null_pointers();
    should_weblist_is_balanced_validate_null_pointers();

    should_create_a_weblist_of_level_zero();
    should_create_a_weblist_of_level_1();
    should_create_a_weblist_of_level_2();
    should_create_a_weblist_of_custom_type_of_level_3();
    should_destruct_a_weblist_and_set_pointer_to_null();
    should_add_a_bunch_of_integers_into_a_weblist_of_level_1_and_keep_balance();
    // should_add_a_bunch_of_custom_types_into_a_weblist_of_level_1_and_keep_balance();
    // should_remove_a_bunch_of_integers_into_a_weblist_of_level_1_and_keep_balance();
    // should_search_some_data_into_a_weblist_of_level_1();
    // should_walk_through_a_weblist_of_level_1();
    // should_replace_a_branch_of_weblist_and_keep_balance();
    should_count_the_total_of_keys_from_a_weblist();
    // should_count_the_total_of_values_from_a_weblist();
    should_get_a_list_of_keys_from_a_weblist();

    printf("SUCCESS\n");
    return 0;
}

void should_weblist_create_validate_null_pointers() {
    weblist_p weblist_ok = NULL;
    weblist_p weblist_nok = (weblist_p) 0x11;
    assert(weblist_create(NULL, 0, 4) != SUCCESS, __func__);
    assert(weblist_create(&weblist_ok, 0, 0) != SUCCESS, __func__);
    assert(weblist_create(&weblist_nok, 0, 4) != SUCCESS, __func__);
}

void should_weblist_destruct_validate_null_pointers() {
    weblist_p weblist_nok = NULL;
    assert(weblist_destruct(NULL) != SUCCESS, __func__);
    assert(weblist_destruct(&weblist_nok) != SUCCESS, __func__);
}

void should_weblist_add_data_validate_null_pointers() {
    weblist_p weblist = (weblist_p) 0x11;
    int data;
    assert(weblist_add_data(NULL, &data, compare_int) != SUCCESS, __func__);
    assert(weblist_add_data(weblist, NULL, compare_int) != SUCCESS, __func__);
    assert(weblist_add_data(weblist, &data, NULL) != SUCCESS, __func__);
}

void should_weblist_remove_data_validate_null_pointers() {
    weblist_p weblist = (weblist_p) 0x11;
    int data;
    assert(weblist_remove_data(NULL, &data, compare_int) != SUCCESS, __func__);
    assert(weblist_remove_data(weblist, NULL, compare_int) != SUCCESS, __func__);
    assert(weblist_remove_data(weblist, &data, NULL) != SUCCESS, __func__);
}

void should_weblist_search_data_validate_null_pointers() {
    weblist_p weblist = (weblist_p) 0x11;
    int data;
    assert(weblist_search_data(NULL, &data, compare_int) != SUCCESS, __func__);
    assert(weblist_search_data(weblist, NULL, compare_int) != SUCCESS, __func__);
    assert(weblist_search_data(weblist, &data, NULL) != SUCCESS, __func__);
}

void should_weblist_walk_data_validate_null_pointers() {
    weblist_p weblist = (weblist_p) 0x11;
    assert(weblist_walk_data(NULL, process_int) != SUCCESS, __func__);
    assert(weblist_walk_data(weblist, NULL) != SUCCESS, __func__);
}

void should_weblist_copy_list_by_key_validate_null_pointers() {
    weblist_p weblist = (weblist_p) 0x11;
    pDDLL list = (pDDLL) 0x11;
    assert(weblist_copy_list_by_key(NULL, 1, &list) != SUCCESS, __func__);
    assert(weblist_copy_list_by_key(weblist, -1, &list) != SUCCESS, __func__);
    assert(weblist_copy_list_by_key(weblist, 0, NULL) != SUCCESS, __func__);
}

void should_weblist_replace_list_by_key_validate_null_pointers() {
    weblist_p weblist = (weblist_p) 0x11;
    pDDLL list = (pDDLL) 0x11;
    assert(weblist_replace_list_by_key(NULL, 1, list, compare_int) != SUCCESS, __func__);
    assert(weblist_replace_list_by_key(weblist, -1, list, compare_int) != SUCCESS, __func__);
    assert(weblist_replace_list_by_key(weblist, 0, NULL, compare_int) != SUCCESS, __func__);
    assert(weblist_replace_list_by_key(weblist, 0, list, NULL) != SUCCESS, __func__);
}

void should_weblist_remove_list_by_key_validate_null_pointers() {
    weblist_p weblist = (weblist_p) 0x11;
    pDDLL list = (pDDLL) 0x11;
    assert(weblist_remove_list_by_key(NULL, 1, &list, compare_int) != SUCCESS, __func__);
    assert(weblist_remove_list_by_key(weblist, -1, &list, compare_int) != SUCCESS, __func__);
    assert(weblist_remove_list_by_key(weblist, 0, NULL, compare_int) != SUCCESS, __func__);
    assert(weblist_remove_list_by_key(weblist, 0, &list, NULL) != SUCCESS, __func__);
}

void should_weblist_count_by_key_validate_null_pointers() {
    weblist_p weblist = (weblist_p) 0x11;
    int count;
    assert(weblist_count_by_key(NULL, 0, &count) != SUCCESS, __func__);
    assert(weblist_count_by_key(weblist, -1, &count) != SUCCESS, __func__);
    assert(weblist_count_by_key(weblist, 1, NULL) != SUCCESS, __func__);
}

void should_weblist_total_of_keys_validate_null_pointers() {
    weblist_p weblist = (weblist_p) 0x11;
    int count;
    assert(weblist_total_of_keys(NULL, &count) != SUCCESS, __func__);
    assert(weblist_total_of_keys(weblist, NULL) != SUCCESS, __func__);
}

void should_weblist_count_validate_null_pointers() {
    weblist_p weblist = (weblist_p) 0x11;
    int count;
    assert(weblist_count(NULL, &count) != SUCCESS, __func__);
    assert(weblist_count(weblist, NULL) != SUCCESS, __func__);
}

void should_weblist_get_keys_validate_null_pointers() {
    weblist_p weblist = (weblist_p) 0x11;
    pDDLL list_of_keys = (pDDLL) 0x11;
    assert(weblist_get_keys(NULL, &list_of_keys) != SUCCESS, __func__);
    assert(weblist_get_keys(weblist, NULL) != SUCCESS, __func__);
}

void should_weblist_is_balanced_validate_null_pointers() {
    assert(weblist_is_balanced(NULL) != SUCCESS, __func__);
}

void should_create_a_weblist_of_level_zero() {
    weblist_p weblist = NULL;
    assert(weblist_create(&weblist, 0, 4) == SUCCESS, __func__);
    assert(weblist_destruct(&weblist) == SUCCESS, __func__);
}

void should_create_a_weblist_of_level_1() {
    weblist_p weblist = NULL;
    assert(weblist_create(&weblist, 1, 4) == SUCCESS, __func__);
    assert(weblist_destruct(&weblist) == SUCCESS, __func__);
}

void should_create_a_weblist_of_level_2() {
    weblist_p weblist = NULL;
    assert(weblist_create(&weblist, 2, 4) == SUCCESS, __func__);
    assert(weblist_destruct(&weblist) == SUCCESS, __func__);
}

void should_create_a_weblist_of_custom_type_of_level_3() {
    weblist_p weblist = NULL;
    assert(weblist_create(&weblist, 3, sizeof(test_type_t)) == SUCCESS, __func__);
    assert(weblist_destruct(&weblist) == SUCCESS, __func__);
}

void should_destruct_a_weblist_and_set_pointer_to_null() {
    weblist_p weblist = NULL;
    assert(weblist_create(&weblist, 0, 4) == SUCCESS, __func__);
    assert(weblist_destruct(&weblist) == SUCCESS, __func__);
    assert(weblist == NULL, __func__);
}

void should_add_a_bunch_of_integers_into_a_weblist_of_level_1_and_keep_balance() {
    const int num_elements = 64 * 512;
    weblist_p weblist = NULL;
    pDDLL check_list_1 = NULL;
    int check_results_1;
    pDDLL check_list_2 = NULL;
    int check_results_2;
    assert(weblist_create(&weblist, 1, sizeof(int)) == SUCCESS, __func__);

    for (size_t i = 0; i < 1; i++) {
        assert(weblist_add_data(weblist, &i, compare_int) == SUCCESS, __func__);
    }
    
    assert(weblist_is_balanced(weblist) == SUCCESS, __func__);
    assert(weblist_copy_list_by_key(weblist, 0, &check_list_1) == SUCCESS, __func__);
    assert(weblist_copy_list_by_key(weblist, 63, &check_list_2) == SUCCESS, __func__);

    for (size_t i = 0; i < 512; i++) {
        rBegin(check_list_1, &check_results_1);
        rBegin(check_list_2, &check_results_2);

        assert(check_results_1 >= 0 && check_results_1 <= 512, __func__);
        assert(check_results_2 >= (63 * 512) && check_results_2 <= (64 * 512), __func__);
    }

    assert(weblist_destruct(&weblist) == SUCCESS, __func__);

    dDDLL(&check_list_1);
    dDDLL(&check_list_2);
}

void should_add_a_bunch_of_custom_types_into_a_weblist_of_level_1_and_keep_balance() {
    const int num_elements = 64 * 512;
    test_type_t element;
    weblist_p weblist = NULL;
    pDDLL check_list_1 = NULL;
    test_type_t check_results_1;
    pDDLL check_list_2 = NULL;
    test_type_t check_results_2;
    assert(weblist_create(&weblist, 1, sizeof(test_type_t)) == SUCCESS, __func__);

    for (size_t i = 0; i < num_elements; i++) {
        element.a = i;
        element.b = i;
        assert(weblist_add_data(weblist, &element, compare_test_type) == SUCCESS, __func__);
    }
    
    assert(weblist_is_balanced(weblist) == SUCCESS, __func__);
    assert(weblist_copy_list_by_key(weblist, 0, &check_list_1) == SUCCESS, __func__);
    assert(weblist_copy_list_by_key(weblist, 63, &check_list_2) == SUCCESS, __func__);

    for (size_t i = 0; i < 512; i++) {
        rBegin(check_list_1, &check_results_1);
        rBegin(check_list_2, &check_results_2);

        assert(check_results_1.a >= 0 && check_results_1.a <= 512, __func__);
        assert(check_results_1.b >= 0 && check_results_1.b <= 512, __func__);
        assert(check_results_2.a >= (63 * 512) && check_results_2.a <= (64 * 512), __func__);
        assert(check_results_2.b >= (63 * 512) && check_results_2.b <= (64 * 512), __func__);
    }

    assert(weblist_destruct(&weblist) == SUCCESS, __func__);

    dDDLL(&check_list_1);
    dDDLL(&check_list_2);
}

void should_remove_a_bunch_of_integers_into_a_weblist_of_level_1_and_keep_balance() {
    const int num_elements = 64 * 512;
    weblist_p weblist = NULL;
    pDDLL check_list_1 = NULL;
    int check_results_1;
    pDDLL check_list_2 = NULL;
    int check_results_2;
    assert(weblist_create(&weblist, 1, sizeof(int)) == SUCCESS, __func__);

    for (size_t i = 0; i < num_elements; i++) {
        assert(weblist_add_data(weblist, &i, compare_int) == SUCCESS, __func__);
    }

    assert(weblist_remove_list_by_key(weblist, 0, &check_list_1, compare_int) == SUCCESS, __func__);
    assert(weblist_remove_list_by_key(weblist, 31, &check_list_2, compare_int) == SUCCESS, __func__);
    assert(weblist_is_balanced(weblist) == SUCCESS, __func__);

    for (size_t i = 0; i < 512; i++) {
        rBegin(check_list_1, &check_results_1);
        assert(check_results_1 >= 0 && check_results_1 <= 512, __func__);
    }

    for (size_t i = 0; i < 504; i++) {
        rBegin(check_list_2, &check_results_2);
        assert(check_results_2 >= (16136) && check_results_2 <= (16136 + 504), __func__);
    }

    assert(weblist_destruct(&weblist) == SUCCESS, __func__);

    dDDLL(&check_list_1);
    dDDLL(&check_list_2);
}

void should_search_some_data_into_a_weblist_of_level_1() {
    const int num_elements = 64 * 512;
    weblist_p weblist = NULL;
    assert(weblist_create(&weblist, 1, sizeof(int)) == SUCCESS, __func__);

    for (size_t i = 0; i < num_elements; i++) {
        assert(weblist_add_data(weblist, &i, compare_int) == SUCCESS, __func__);
    }
    
    assert(weblist_is_balanced(weblist) == SUCCESS, __func__);

    for (size_t i = 0; i < num_elements; i++) {
        assert(weblist_search_data(weblist, &i, compare_int) == SUCCESS, __func__);
    }

    assert(weblist_destruct(&weblist) == SUCCESS, __func__);
}

void should_walk_through_a_weblist_of_level_1() {
    const int num_elements = 64 * 512;
    int elements[num_elements];
    weblist_p weblist = NULL;
    assert(weblist_create(&weblist, 1, sizeof(int)) == SUCCESS, __func__);
    process_int_init(num_elements);

    for (size_t i = 0; i < num_elements; i++) {
        elements[i] = i;
        assert(weblist_add_data(weblist, &elements[i], compare_int) == SUCCESS, __func__);
    }

    assert(weblist_walk_data(weblist, process_int) == SUCCESS, __func__);
    assert(memcmp(process_int_walkthrough(), &elements, sizeof(int) * num_elements) == 0, __func__);

    process_int_destruct();
    assert(weblist_destruct(&weblist) == SUCCESS, __func__);
}

void should_replace_a_branch_of_weblist_and_keep_balance() {
    const int num_elements = 64 * 512;
    int elements[num_elements];
    weblist_p weblist = NULL;
    pDDLL replace_list = NULL;
    assert(weblist_create(&weblist, 1, sizeof(int)) == SUCCESS, __func__);
    cDDLL(&replace_list, sizeof(int));

    for (size_t i = 0; i < num_elements; i++) {
        elements[i] = i;
    }

    for (size_t i = 0; i < (256 * 64); i++) {
        assert(weblist_add_data(weblist, &elements[i], compare_int) == SUCCESS, __func__);
    }

    for (size_t i = (256 * 64); i < num_elements; i++) {
        iEnd(replace_list, &elements[i]);
    }
    
    assert(weblist_is_balanced(weblist) == SUCCESS, __func__);
    assert(weblist_replace_list_by_key(weblist, 32, replace_list, compare_int) == SUCCESS, __func__);
    assert(weblist_is_balanced(weblist) == SUCCESS, __func__);

    for (size_t i = (256 * 64); i < num_elements; i++) {
        assert(weblist_search_data(weblist, &elements[i], compare_int) == SUCCESS, __func__);
    }

    assert(weblist_destruct(&weblist) == SUCCESS, __func__);

    dDDLL(&replace_list);
}

void should_count_the_total_of_keys_from_a_weblist() {
    weblist_p weblist = NULL;
    int count_result;
    assert(weblist_create(&weblist, 2, sizeof(int)) == SUCCESS, __func__);
    assert(weblist_total_of_keys(weblist, &count_result) == SUCCESS, __func__);
    assert(count_result == (8 * 8 * 8), __func__);
    assert(weblist_destruct(&weblist) == SUCCESS, __func__);
}

void should_count_the_total_of_values_from_a_weblist() {
    const int num_elements = 64 * 512;
    weblist_p weblist = NULL;
    int count = 0;

    assert(weblist_create(&weblist, 1, sizeof(int)) == SUCCESS, __func__);

    for (size_t i = 0; i < num_elements; i++) {
        assert(weblist_add_data(weblist, &i, compare_int) == SUCCESS, __func__);
    }
    
    assert(weblist_count(weblist, &count) == SUCCESS, __func__);
    assert(count == num_elements, __func__);

    assert(weblist_destruct(&weblist) == SUCCESS, __func__);
}

void should_get_a_list_of_keys_from_a_weblist() {
    weblist_p weblist = NULL;
    pDDLL list_of_keys = NULL;
    int key;
    assert(weblist_create(&weblist, 2, sizeof(int)) == SUCCESS, __func__);
    assert(weblist_get_keys(weblist, &list_of_keys) == SUCCESS, __func__);
    
    for (size_t i = 0; i < (8 * 8 * 8); i++) {
        rBegin(list_of_keys, &key);
        assert(key == i, __func__);
    }
    
    assert(weblist_destruct(&weblist) == SUCCESS, __func__);

    dDDLL(&list_of_keys);
}
