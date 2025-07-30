#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "kd_tree.h"

struct l_node{
	struct k_node data;
	struct l_node* next;
};

struct l_node* new_l_node(struct k_node* data);
struct l_node* insert_at_start(struct l_node* root, struct k_node* data);
void free_l_list(struct l_node** root);



#endif
