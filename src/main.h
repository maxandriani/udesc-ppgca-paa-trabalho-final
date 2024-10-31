#include <stdlib.h>
#include "DDLL_pub.h"
#include "weblist_pub.h"

void should_not_create_a_weblist_from_null_pointer();
void should_only_create_a_weblist_from_null_ref();
void should_not_add_items_on_weblist_from_null_pointer();
void should_create_a_weblist_of_level_3();
void should_add_itens_on_a_weblist_of_level_2_and_keep_balance();