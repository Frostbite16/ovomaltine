#include "query_store.h"
#include "generic_context.h"
#include "kd_tree.h"
#include "linked_list.h"
#include "file_handle.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.141592653
#endif


struct l_node* get_lowest_price(uint8_t fuel_type, struct l_node* root){
	struct l_node* lowest_node = NULL;
	
	struct l_node* current_node = root;
	while(current_node){
		if(current_node->data.fuel_type == fuel_type){
			if(!lowest_node) lowest_node = current_node;
			else{
				if(current_node->data.st_data.fuel_price < lowest_node->data.st_data.fuel_price)
					lowest_node = current_node;
			}	
		}
		current_node = current_node->next;
	}
	return lowest_node;
}

void store_into_file(struct gs_generic_protocol_t* insertion_data){
	FILE* bin_file = fopen("gas_station_info.bin", "rb");
	struct k_node* kdt_root = NULL;
	
	if(!insertion_data || !insertion_data->data_protocol) return;
	struct gs_data_protocol_t* gs_data = insertion_data->data_protocol;

	if(bin_file){ 
		read_bin_file(bin_file, &kdt_root);
		fclose(bin_file);
	}
	kdt_root = insert_rec(kdt_root, insertion_data->fuel_type, gs_data, 0);
	if((bin_file = fopen("gas_station_info.bin", "wb")) == 0){
		perror("fopen");
		exit(EXIT_FAILURE);
	}
	write_bin_file(bin_file, kdt_root);
	free_tree(&kdt_root);
	fclose(bin_file);
	return;
}

struct k_node* search_tree(struct gs_generic_protocol_t* query_data){
	FILE* bin_file = fopen("gas_station_info.bin", "rb");
	struct k_node* kdt_root = NULL;
	struct k_node* lowest_node = NULL;
	struct l_node* linked_list = NULL;

	if(!query_data || !query_data->search_protocol) return NULL;
	struct gs_search_protocol_t* gs_query = query_data->search_protocol;
		
	if(bin_file){
		read_bin_file(bin_file, &kdt_root);
		fclose(bin_file);

		range_search_rec(kdt_root, gs_query->latitude, gs_query->longitude, gs_query->radius, &linked_list, 0);
		struct l_node* lowest_node_l = get_lowest_price(query_data->fuel_type, linked_list);
		
		if(lowest_node_l){ 
			lowest_node = malloc(sizeof(struct k_node));
			*lowest_node = lowest_node_l->data;
			lowest_node->left = NULL;
			lowest_node->right = NULL;
		}
		free_tree(&kdt_root);
		free_l_list(&linked_list);
	}
	return lowest_node;
} 





