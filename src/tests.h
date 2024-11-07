#include <stdlib.h>
#include "DDLL_pub.h"
#include "weblist_pub.h"
#include <memory.h>

typedef struct test_type_t {
    int a;
    int b;
} test_type_t;

void assert(int expression, const char fn_name[]);

int compare_int(void *a, void *b);
int process_int(void *x, int key);
int compare_test_type(void *a, void *b);
int process_test_type(void *x, int key);
void process_int_init(size_t count);
void process_int_destruct();
int *process_int_walkthrough();

void should_weblist_create_validate_null_pointers();
void should_weblist_destruct_validate_null_pointers();
void should_weblist_add_data_validate_null_pointers();
void should_weblist_remove_data_validate_null_pointers();
void should_weblist_search_data_validate_null_pointers();
void should_weblist_walk_data_validate_null_pointers();
void should_weblist_copy_list_by_key_validate_null_pointers();
void should_weblist_replace_list_by_key_validate_null_pointers();
void should_weblist_remove_list_by_key_validate_null_pointers();
void should_weblist_create_list_by_key_validate_null_pointers();
void should_weblist_count_by_key_validate_null_pointers();
void should_weblist_total_of_keys_validate_null_pointers();
void should_weblist_count_validate_null_pointers();
void should_weblist_get_keys_validate_null_pointers();
void should_weblist_is_balanced_validate_null_pointers();

void should_create_a_weblist_of_level_zero();
void should_create_a_weblist_of_level_1();
void should_create_a_weblist_of_level_2();
void should_create_a_weblist_of_custom_type_of_level_3();
void should_destruct_a_weblist_and_set_pointer_to_null();
void should_add_a_bunch_of_integers_into_a_weblist_of_level_2_and_keep_balance();
void should_add_a_bunch_of_custom_types_into_a_weblist_of_level_2_and_keep_balance();
void should_remove_a_bunch_of_integers_into_a_weblist_of_level_2_and_keep_balance();
void should_search_some_data_into_a_weblist_of_level_2();
void should_walk_through_a_weblist_of_level_2();
void should_replace_a_branch_of_weblist_and_keep_balance();
void should_create_a_branch_of_weblist_by_key();
void should_count_the_total_of_keys_from_a_weblist();
void should_count_the_total_of_values_from_a_weblist();
void should_get_a_list_of_keys_from_a_weblist();
