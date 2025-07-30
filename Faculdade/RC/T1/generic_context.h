#ifndef GENERIC_CONTEXT_H
#define GENERIC_CONTEXT_H

#include <stdint.h>
#include <netinet/in.h>

#define EXPECTED_MESSAGE_SIZE 18

struct gs_data_protocol_t{
	uint32_t fuel_price; // Multiplicado por 1000;
	float latitude; // (primeiro bit) N ou S, 8 bits - grau, mantissa - minutos e segundos (big endian)
	float longitude; // (primeiro bit) W ou E, 8 bits - grau, mantissa - minutos e segundos (big endian)
};

struct gs_search_protocol_t{
	uint32_t radius;
	float latitude;
	float longitude;
};

struct gs_generic_protocol_t{
	char message_type;
	int32_t message_ID;
	uint8_t fuel_type;
	struct gs_data_protocol_t* data_protocol;
	struct gs_search_protocol_t* search_protocol;
};

enum rdt_check{NACK, ACK};

#endif