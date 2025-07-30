#include <netinet/in.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "client_utils.h"
#include "generic_context.h"

#define DEG_MASK		0xFF
#define MIN_MASK		0x3F
#define SEC_INT_MASK	0x3F
#define SEC_FRAC_MASK	0x7FF

#define DEG_SHIFT		23
#define MIN_SHIFT		17
#define SEC_INT_SHIFT	11


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

//
//	Recebe ponteiro estrutura vazia de mensagem gs_generic_protocol_t
//
void get_message(struct gs_generic_protocol_t* new_message) { 
	//new_message->message_type = get_message_type();
	
	char message_type;
	uint8_t fuel_type;
	uint32_t generic_int; // Pode ser o preço ou o raio
	float latitude, longitude;

	printf("Insira os dados: ");
	scanf("%c %hhu %u %f %f", &message_type, &fuel_type, &generic_int, &latitude, &longitude);

	//printf("Tipo de combustivel: ");
	//scanf("%hhu", &new_message->fuel_type);
	if(fuel_type > 2) {
		fprintf(stderr, "Tipo de combustivel invalido\n");
		exit(EXIT_FAILURE);
	}
	
	// Salva as informações na struct
	if (message_type == 'D' || message_type == 'd') { 
		struct gs_data_protocol_t* data_protocol = new_message->data_protocol;
		new_message->message_type = 'D';
		new_message->fuel_type = fuel_type;
		data_protocol->fuel_price = generic_int;
		data_protocol->latitude = latitude;
		data_protocol->longitude = longitude;
	}
	else if(message_type == 'P' || message_type == 'p') {
		struct gs_search_protocol_t* search_protocol = new_message->search_protocol;
		new_message->message_type = 'P';
		new_message->fuel_type = fuel_type;
		search_protocol->radius = generic_int;
		search_protocol->latitude = latitude;
		search_protocol->longitude = longitude;
	}
	else {
		fprintf(stderr, "Tipo de mensagem invalido\n");
		exit(EXIT_FAILURE);
	}

}

// Função converte a mensagem em big endian !! Só funciona se segue a estrutura exata do protocolo
void convert_message_ns(char* message_str) {
	char* message_point = message_str;

	// Converte o tamanho da mensagem
	const uint32_t message_size_ns = htonl(*(uint32_t *)message_point);
	memcpy(message_point, &message_size_ns, sizeof(uint32_t)); // Converte o tamanho da mensagem
	message_point += sizeof(uint32_t) + 2*sizeof(uint8_t);

	const uint32_t generic_ns = htonl(*(uint32_t *)message_point); // Pode ser o raio ou o preço do combustivel
	memcpy(message_point, &generic_ns, sizeof(uint32_t));
	message_point += sizeof(uint32_t);

	const int32_t latitude_ns = htonl(*(int32_t *)message_point);
	memcpy(message_point, &latitude_ns, sizeof(int32_t)); // Converte a latitude
	message_point += sizeof(uint32_t);

	const int32_t longitude_ns = htonl(*(int32_t *)message_point);
	memcpy(message_point, &longitude_ns, sizeof(int32_t)); // Converte a longitude

}

// Empacota a mensagem em uma string não legivel, a string contém somente os bytes
void pack_message(const struct gs_generic_protocol_t* message_st, char* message_str) {
	uint32_t const message_size = EXPECTED_MESSAGE_SIZE; // Já foi util em alguma versão do programa, porém não tem mais nenhum uso, mas daria muito trabalho remover
	char* message_point = message_str;
	if (message_st) {

		// Escreve um dado de N bytes na string
		memcpy(message_point, &message_size, sizeof(message_size));
		message_point += sizeof(message_size); // O ponteiro pra string sinaliza o proximo byte vazio da string

		memcpy(message_point, &message_st->message_type, sizeof(message_st->message_type));
		message_point += sizeof(message_st->message_type);

		memcpy(message_point, &message_st->fuel_type, sizeof(message_st->fuel_type));
		message_point += sizeof(message_st->fuel_type);

		if (message_st->message_type == 'D') {
			const struct gs_data_protocol_t data_protocol = *message_st->data_protocol;
			memcpy(message_point, &data_protocol.fuel_price, sizeof(data_protocol.fuel_price));
			message_point += sizeof(data_protocol.fuel_price);

			memcpy(message_point, &data_protocol.latitude, sizeof(data_protocol.latitude));
			message_point += sizeof(data_protocol.latitude);

			memcpy(message_point, &data_protocol.longitude, sizeof(data_protocol.longitude));
		}
		else if(message_st->message_type == 'P') {
			const struct gs_search_protocol_t search_protocol = *message_st->search_protocol;
			memcpy(message_point, &search_protocol.radius, sizeof(search_protocol.radius));
			message_point += sizeof(search_protocol.radius);

			memcpy(message_point, &search_protocol.latitude, sizeof(search_protocol.latitude));
			message_point += sizeof(search_protocol.latitude);

			memcpy(message_point, &search_protocol.longitude, sizeof(search_protocol.longitude));
		}

		convert_message_ns(message_str); // Chama a rotina para conversão dos dados para BIG ENDIAN
		return;
	}

	fprintf(stderr, "Erro: estrutura de mensagem vazia\n");
	exit(EXIT_FAILURE);

}

