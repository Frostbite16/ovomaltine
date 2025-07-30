#ifndef FILE_HANDLE_H
#define FILE_HANDLE_H

#include "kd_tree.h"
#include <stdint.h>
#include <stdio.h>

struct k_node_arq{
	uint8_t fuel_type;
	struct gs_data_protocol_t st_data;
	uint8_t left, right;
};

void create_file_node(struct k_node_arq* node, struct k_node* kdt_node);
void write_bin_file(FILE* bin_file, struct k_node* kdt_root);
void read_bin_file(FILE* bin_file, struct k_node** kdt_root);

#endif