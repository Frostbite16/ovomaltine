#include "linked_list.h"
#include <stdlib.h>

struct l_node* new_l_node(struct k_node* data){
	struct l_node* new_node = NULL;

	if(!data) return NULL;
	if((new_node = malloc(sizeof(struct l_node))) == 0) return NULL;

	new_node->data = *data;
	new_node->next = NULL;

	return new_node;
}

struct l_node* insert_at_start(struct l_node* root, struct k_node* data){
	
	if(root == NULL) return new_l_node(data);
	
	struct l_node* current_node = root;
	struct l_node* new_node = new_l_node(data);

	new_node->next = current_node;

	return new_node;
}

void free_l_list(struct l_node** root){
	struct l_node* current_node = *root;

	if(!current_node) return;
	
	free_l_list(&current_node->next);
	
	free(current_node);
}


