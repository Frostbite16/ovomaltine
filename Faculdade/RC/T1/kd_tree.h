#ifndef KD_TREE_H
#define KD_TREE_H

#include "generic_context.h"

struct l_node;

struct k_node{
	uint8_t fuel_type;
	struct gs_data_protocol_t st_data;
	struct k_node *left, *right;
};

struct k_node* new_k_node(uint8_t fuel_type, struct gs_data_protocol_t* data);
struct k_node* insert_rec(struct k_node* root, uint8_t fuel_type, struct gs_data_protocol_t* data, unsigned depth);
struct k_node* insert(struct k_node* root, uint8_t fuel_type, struct gs_data_protocol_t* data);
short are_points_the_same(struct gs_data_protocol_t* data1, struct gs_data_protocol_t* data2);
short are_nodes_the_same(struct k_node* node1, struct k_node* node2);
short search_rec(struct k_node* root, struct gs_data_protocol_t* data, unsigned depth);
short search(struct k_node* root, struct gs_data_protocol_t* data);
void free_tree(struct k_node** root);
float haversine_distance(float lat1, float lon1, float lat2, float lon2);
short is_within_distance(struct gs_search_protocol_t* user_search, struct gs_data_protocol_t* data, uint32_t search_radius);
void range_search_rec(struct k_node* root, float query_lat, float query_lon, float radius, struct l_node** buf, unsigned depth);

#endif
