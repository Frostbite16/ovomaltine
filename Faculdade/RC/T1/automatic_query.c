#include <stdio.h>
#include <stdlib.h>
#include "generic_context.h"


struct gs_generic_protocol_t* read_data_from_file(FILE* bin_file){
	if(bin_file){
		char message_type;
		uint8_t fuel_type;
		uint32_t generic_fr;
		float latitude;
		float longitude;
		if(fscanf(bin_file, " %c %hhu %u %f %f", &message_type, &fuel_type, &generic_fr, &latitude, &longitude) == 5){
			struct gs_generic_protocol_t* new_protocol = malloc(sizeof(struct gs_generic_protocol_t));
			new_protocol->message_type = message_type;
			new_protocol->fuel_type = fuel_type;
			if (message_type == 'D') {
				struct gs_data_protocol_t* new_data = malloc(sizeof(struct gs_data_protocol_t));
				new_protocol->data_protocol = new_data;
				new_data->latitude = latitude;
				new_data->longitude = longitude;
				new_data->fuel_price = generic_fr;
				return new_protocol;
			}
			else if (message_type == 'P') {
				struct gs_search_protocol_t* new_search = malloc(sizeof(struct gs_search_protocol_t));
				new_protocol->search_protocol = new_search;
				new_search->latitude = latitude;
				new_search->longitude = longitude;
				new_search->radius = generic_fr;
				return new_protocol;
			}
			else {
				free(new_protocol);
				return NULL;
			}
		}
		else return NULL;
	}
	else {
		printf("[ERRO] Arquivo de dados inexistente");
		return NULL;
	}


}





