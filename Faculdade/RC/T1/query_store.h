#ifndef QUERY_STORE_H
#define QUERY_STORE_H

#include "generic_context.h"
#include "kd_tree.h"
#include "linked_list.h"

struct l_node* get_lowest_price(uint8_t fuel_type, struct l_node* root);
void store_into_file(struct gs_generic_protocol_t* insertion_data);
struct k_node* search_tree(struct gs_generic_protocol_t* query_data);

#endif //QUERY_STORE_H
