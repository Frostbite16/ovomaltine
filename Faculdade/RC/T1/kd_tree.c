#include "generic_context.h"
#include "kd_tree.h"
#include "linked_list.h"
#include <stdlib.h>
#include <math.h>


#ifndef M_PI
#define M_PI 3.141592653
#endif

struct k_node* new_k_node(uint8_t fuel_type, struct gs_data_protocol_t* data){
	struct k_node* new_node = NULL;

	if(!data) return NULL;
	if((new_node = malloc(sizeof(struct k_node))) == 0)	return NULL;
	
	new_node->fuel_type = fuel_type;
	new_node->st_data = *data;
	new_node->left = new_node->right = NULL;
	
	return new_node;
}

struct k_node* insert_rec(struct k_node* root, uint8_t fuel_type, struct gs_data_protocol_t* data, unsigned depth){

	if (root == NULL){
		return new_k_node(fuel_type, data);
	}

	unsigned cd = depth % 2;
	float coord_root = (cd == 0 ? root->st_data.latitude : root->st_data.longitude);
	float coord_new = (cd == 0 ? data->latitude : data->longitude);

	if(coord_new < coord_root)
		root->left = insert_rec(root->left, fuel_type, data, depth+1);
	else
		root->right = insert_rec(root->right, fuel_type, data, depth+1);

	return root;
}

struct k_node* insert(struct k_node* root, uint8_t fuel_type, struct gs_data_protocol_t* data){
	return insert_rec(root, fuel_type, data, 0);
}

short are_points_the_same(struct gs_data_protocol_t* data1, struct gs_data_protocol_t* data2){
	return (data1->latitude == data2->latitude && data1->longitude == data2->longitude);
}

short are_nodes_the_same(struct k_node* node1, struct k_node* node2){
	return (node1->fuel_type == node2->fuel_type &&
			node1->st_data.longitude == node2->st_data.longitude &&
			node1->st_data.latitude == node2->st_data.latitude);
}

short search_rec(struct k_node* root, struct gs_data_protocol_t* data, unsigned depth){
	if(root==NULL) return 0;
	if(are_points_the_same(&root->st_data, data)) return 1;

	unsigned cd = depth % 2;
	float coord_root = (cd == 0 ? root->st_data.latitude : root->st_data.longitude);
	float coord_new = (cd == 0 ? data->latitude : data->longitude);

	if(coord_new < coord_root)
		return search_rec(root->left, data, depth+1);


	return search_rec(root->right, data, depth + 1);
}

short search(struct k_node* root, struct gs_data_protocol_t* data){
	return search_rec(root, data, 0);
}

void free_tree(struct k_node** root){
	if(root == NULL || *root == NULL) return;
	
	struct k_node* current_node = *root;

	free_tree(&current_node->right);
	free_tree(&current_node->left);

	free(current_node);
}

float haversine_distance(float lat1, float lon1, float lat2, float lon2){
	const float earth_radius = 6371e3;
	const float lat1_rad = lat1 * (float)M_PI/180;
	const float lat2_rad = lat2 * (float)M_PI/180;
	const float delta_lat = (lat2 - lat1) * (float)M_PI/180;
	const float delta_lon = (lon2 - lon1) * (float)M_PI/180;

	const float a = sinf(delta_lat/2) * sinf(delta_lat/2) + cosf(lat1_rad) * cosf(lat2_rad) * sinf(delta_lon/2) * sinf(delta_lon/2);
	
	const float c = 2 * atan2f(sqrt(a), sqrt(1-a));

	return earth_radius * c;
}

short is_within_distance(struct gs_search_protocol_t* user_search, struct gs_data_protocol_t* data, uint32_t search_radius){
	float lat1 = user_search->latitude;
	float lon1 = user_search->longitude;
	float lat2 = data->latitude;
	float lon2 = data->longitude;

	return (search_radius >= haversine_distance(lat1, lon1, lat2, lon2) ? 1 : 0);
}

void range_search_rec(struct k_node* root, float query_lat, float query_lon, float radius, struct l_node** buf, unsigned depth){
	if(!root) return;
	
	float d = haversine_distance(query_lat, query_lon, root->st_data.latitude, root->st_data.longitude);
	
	if (d <= radius){
		*buf = insert_at_start(*buf, root);
	}

	unsigned axis = depth % 2;
	float query_coord = (axis == 0 ? query_lat : query_lon);
	float node_coord = (axis == 0 ? root->st_data.latitude : root->st_data.longitude);

	struct k_node* near_subtree = (query_coord < node_coord ? root->left : root->right);
	struct k_node* far_subtree = (query_coord < node_coord ? root->right : root->left);

	range_search_rec(near_subtree, query_lat, query_lon, radius, buf, depth + 1);
	range_search_rec(far_subtree, query_lat, query_lon, radius, buf, depth + 1);
}


