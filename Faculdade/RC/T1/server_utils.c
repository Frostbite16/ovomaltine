
#include <inttypes.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include "server_utils.h"
#include "generic_context.h"
#include "query_store.h"


pthread_rwlock_t message_lock = PTHREAD_RWLOCK_INITIALIZER;

int current_message_id = 0;

uint32_t read_and_convert32(char **p){
	uint32_t net_val;
    memcpy(&net_val, *p, sizeof(net_val));
    uint32_t host_val = ntohl(net_val);
    memcpy(*p, &host_val, sizeof(host_val));
    *p += sizeof(host_val);
	return host_val;
}

float read_and_convert_f(char **p){
	uint32_t net_val = *(uint32_t*)*p;
	uint32_t host_val = ntohl(net_val);
	
	memcpy(*p, &host_val, sizeof(host_val));
	char* aux_point = *p;
	*p += sizeof(host_val);

	return *(float *)aux_point;

}

uint16_t read_and_convert16(char **p){
	uint16_t net_val;
    memcpy(&net_val, *p, sizeof(net_val));
    uint16_t host_val = ntohs(net_val);
    memcpy(*p, &host_val, sizeof(host_val));
    *p += sizeof(host_val);
	return host_val;
}


uint16_t get_checksum(const char* message, uint32_t message_size){ //RFC 1071
	uint32_t checksum_32 = 0;
	if(message_size != 0){
		int count = message_size;
		uint16_t *message_point = (uint16_t *)message;
		while(count > 1){
			checksum_32 += *message_point++;
			count-=2;
		}
		if(count > 0)
			checksum_32 += *((uint8_t *)message_point);
		while(checksum_32 >> 16)
			checksum_32 = (checksum_32 & 0xffff) + (checksum_32 >> 16);

		return ~checksum_32;
	}
	return 0;
}

void receive_message(char *packed_buffer, size_t buffer_size, struct sockaddr_in client_info, int client_socket){
	socklen_t addrlen = sizeof(client_info);
	char rdt_response = NACK;
	
	pthread_rwlock_wrlock(&message_lock);
	current_message_id += 1;
	pthread_rwlock_unlock(&message_lock);

	if(process_message(packed_buffer, buffer_size) == -1){
		printf("[ERRO] Dados corrompidos, enviando solicitação de novos dados ao cliente\n");
		sendto(client_socket, &rdt_response, sizeof(rdt_response), 0, (struct sockaddr *)&client_info, addrlen);
		return;
	}

	rdt_response = ACK;
	sendto(client_socket, &rdt_response, sizeof(rdt_response), 0, (struct sockaddr *)&client_info, addrlen);


	struct gs_generic_protocol_t new_message_st;
	new_message_st.data_protocol = NULL;
	new_message_st.search_protocol = NULL;
	new_message_st.message_ID = current_message_id;


	char* message_point = packed_buffer + sizeof(uint16_t); // Pula checksum
	unpack_message(message_point, buffer_size, &new_message_st);

	if(new_message_st.message_type == 'D') {
		char return_message[1024];

		pthread_rwlock_wrlock(&message_lock);
		store_into_file(&new_message_st);
		pthread_rwlock_unlock(&message_lock);

		sprintf(return_message, "[RESPOSTA] Mensagem recebida. ID:%d", new_message_st.message_ID);
		sendto(client_socket, &return_message, sizeof(return_message), 0, (struct sockaddr *)&client_info, addrlen);

		free(new_message_st.data_protocol);
	}
	else{
		char return_message[1024];

		pthread_rwlock_rdlock(&message_lock);
		struct k_node* lowest_node = search_tree(&new_message_st);
		pthread_rwlock_unlock(&message_lock);

		if(!lowest_node) sprintf(return_message, "Algo deu errado");
		else sprintf(return_message, "[RESPOSTA] A gasolina mais barata custa %f e fica em %f, %f\n", (float)lowest_node->st_data.fuel_price/1000, lowest_node->st_data.latitude, lowest_node->st_data.longitude);

		sendto(client_socket, &return_message, sizeof(return_message), 0, (struct sockaddr *)&client_info, addrlen);
		free(new_message_st.search_protocol);
	}
}

short process_message(char *packed_buffer, size_t buffer_size){
	char* point_to_buf = packed_buffer;
	
	read_and_convert16(&point_to_buf);

	if(buffer_size < sizeof(uint16_t)) return -1;
	
	uint32_t message_size = *((uint32_t *)point_to_buf);
	message_size = ntohl(message_size);

	point_to_buf -= sizeof(uint16_t);
	
	uint16_t recalc_checksum = get_checksum(point_to_buf, message_size + sizeof(uint16_t));
	
	if(!recalc_checksum) return 0;
	return -1;	

}

void unpack_message(char *packed_buffer, size_t buffer_size, struct gs_generic_protocol_t* new_message_st){

	if(buffer_size < sizeof(uint32_t) + 2){
		fprintf(stderr, "Buffer muito pequeno\n");
		exit(EXIT_FAILURE);
	}
	char *p = packed_buffer;
	uint32_t message_size_hl = read_and_convert32(&p);

	if(message_size_hl == EXPECTED_MESSAGE_SIZE){

			char message_type;
			memcpy(&message_type, p, sizeof(message_type));
			p += sizeof(message_type); // Pula o tipo da mensagem e o tipo de combustivel

			new_message_st->message_type = message_type;

			uint8_t fuel_type;
			memcpy(&fuel_type, p, sizeof(fuel_type));
			p += sizeof(fuel_type);

			new_message_st->fuel_type = fuel_type;

			if(message_type == 'D') {
				uint32_t fuel_price_hl = read_and_convert32(&p);
				float	latitude_hl = read_and_convert_f(&p);
				float	longitude_hl = read_and_convert_f(&p);

				new_message_st->data_protocol = malloc(sizeof(struct gs_data_protocol_t));
				new_message_st->data_protocol->fuel_price = fuel_price_hl;
				new_message_st->data_protocol->latitude = latitude_hl;
				new_message_st->data_protocol->longitude = longitude_hl;
			}
			else if(message_type == 'P'){
				uint32_t range_hl = read_and_convert32(&p);
				float	latitude_hl = read_and_convert_f(&p);
				float	longitude_hl = read_and_convert_f(&p);

				new_message_st->search_protocol = malloc(sizeof(struct gs_search_protocol_t));
				new_message_st->search_protocol->radius = range_hl;
				new_message_st->search_protocol->latitude = latitude_hl;
				new_message_st->search_protocol->longitude = longitude_hl;

			}
			else{
				fprintf(stderr, "Tipo de mensagem desconhecido\n");
				exit(EXIT_FAILURE);
			}
	}
	else {
		fprintf(stderr ,"Formato da mensagem invalido\n");
		exit(EXIT_FAILURE);
	}
}



