#include "file_handle.h"
#include <stdlib.h>

void create_file_node(struct k_node_arq* node, struct k_node* kdt_node){
	node->fuel_type = kdt_node->fuel_type;
	node->st_data = kdt_node->st_data;
	node->left = (kdt_node->left == NULL ? 0 : 1);
	node->right = (kdt_node->right == NULL ? 0 : 1);
}

void write_bin_file(FILE* bin_file, struct k_node* kdt_root){
	struct k_node_arq node_arq;
	if(kdt_root != NULL){
		create_file_node(&node_arq, kdt_root);

		fwrite(&node_arq, sizeof(struct k_node_arq), 1, bin_file);
		if(kdt_root->left) write_bin_file(bin_file, kdt_root->left);
		if(kdt_root->right) write_bin_file(bin_file, kdt_root->right);
	}
}

void read_bin_file(FILE* bin_file, struct k_node** kdt_root){
	struct k_node_arq node_arq;
	if(fread(&node_arq, sizeof(struct k_node_arq), 1, bin_file) != 1) {
		*kdt_root = NULL;
		return;
	}
	//*kdt_root = insert(*kdt_root, node_arq.fuel_type, &node_arq.st_data);
	*kdt_root = new_k_node(node_arq.fuel_type, &node_arq.st_data);
	if(node_arq.left == 1) read_bin_file(bin_file, &(*kdt_root)->left);
	if(node_arq.right == 1) read_bin_file(bin_file, &(*kdt_root)->right);
}
