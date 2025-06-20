#include <netinet/in.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "client_utils.h"

#define DEG_MASK		0xFF
#define MIN_MASK		0x3F
#define SEC_INT_MASK	0x3F
#define SEC_FRAC_MASK	0x7FF

#define DEG_SHIFT		23
#define MIN_SHIFT		17
#define SEC_INT_SHIFT	11


uint32_t pack_coordinates(int32_t degrees, uint32_t minutes, float seconds){
	uint32_t packed_data = 0;

	if (degrees < 0){
		packed_data = packed_data | 1<<31;
	}

	packed_data = packed_data | ((uint32_t)abs(degrees) & DEG_MASK) << DEG_SHIFT;
	packed_data = packed_data | (minutes & MIN_MASK) << MIN_SHIFT;

	double sec_int_part;
	double sec_frac_part = modf(seconds, &sec_int_part);

	packed_data = packed_data | ((uint32_t) sec_int_part & SEC_INT_MASK) << SEC_INT_SHIFT;

	uint32_t scaled_frac = (uint32_t)round(sec_frac_part * (SEC_FRAC_MASK + 1));

	packed_data = packed_data | (scaled_frac & SEC_FRAC_MASK);

	return packed_data;

}


char get_message_type(){
	printf("Tipo da mensagem: ");
	char c = getchar();
	while(getchar() != '\n');
	return c;
}

uint32_t get_coordinates_latitude(){
	int32_t degree;
	uint32_t minutes;
	float seconds;
	printf("Digite a latitude: ");
	printf("Graus(Dourados, MS = -22): ");
	scanf("%d", &degree);

	printf("Minutos: ");
	scanf("%u", &minutes);
	if(minutes > 60){
		fprintf(stderr, "Minutos com formato invalido");
		exit(EXIT_FAILURE);
	}

	printf("Segundos: ");
	scanf("%f", &seconds);
	if(seconds < 0.0 || seconds > 60.0){
		fprintf(stderr, "Segundos com formato invalido");
		exit(EXIT_FAILURE);
	}

	return pack_coordinates(degree, minutes, seconds);
}

uint32_t get_coordinates_longitude(){
	int32_t degree;
	uint32_t minutes;
	float seconds;
	printf("Digite a longitude: ");
	printf("Graus(Dourados, MS = -54): ");
	scanf("%d", &degree);

	printf("Minutos: ");
	scanf("%u", &minutes);
	if(minutes > 60){
		fprintf(stderr, "Minutos com formato invalido");
		exit(EXIT_FAILURE);
	}

	printf("Segundos: ");
	scanf("%f", &seconds);
	if(seconds < 0.0 || seconds > 60.0){
		fprintf(stderr, "Segundos com formato invalido");
		exit(EXIT_FAILURE);
	}

	return pack_coordinates(degree, minutes, seconds);
}

void get_input(char* message){
	
	uint32_t message_size = 0;
	char* message_pointer = message + sizeof(message_size); // Garante o espaço no inicio da mensagem para o tamanho
	char message_type, fuel_type_string[16];
	uint8_t fuel_type;

	message_type = get_message_type();

	memcpy(message_pointer, &message_type, sizeof(message_type));
	message_pointer += sizeof(message_type);
	message_size += sizeof(message_type);

	printf("Tipo de combustivel: ");
	scanf("%15s", fuel_type_string);

	if(sscanf(fuel_type_string, "%hhu", &fuel_type) != 1){
		fprintf(stderr, "Entrada invalida para combustivel");
		exit(EXIT_FAILURE);
	}

	if(fuel_type > 2){
		fprintf(stderr, "Tipo de combustivel invalido\n");
		exit(EXIT_FAILURE);
	}

	memcpy(message_pointer, &fuel_type, sizeof(fuel_type));
	message_pointer += sizeof(fuel_type);
	message_size += sizeof(fuel_type);

	if(message_type == 'D'){
		uint32_t fuel_price;
		float fuel_price_f;
		uint32_t latitude, longitude;

		printf("Preço do combustivel: ");
		if(scanf("%f", &fuel_price_f) != 1){
			fprintf(stderr, "Formatação do preço de combustivel invalida");
			exit(EXIT_FAILURE);
		}

		if(fuel_price_f < 0){
			fprintf(stderr, "Valor do combustivel tem que ser positivo");
			exit(EXIT_FAILURE);
		}

		fuel_price = (uint32_t)(fuel_price_f * 1000);

		uint32_t fuel_price_ns = htonl(fuel_price);
		memcpy(message_pointer, &fuel_price_ns, sizeof(fuel_price_ns));
		message_pointer += sizeof(fuel_price_ns);
		message_size += sizeof(fuel_price_ns);

		latitude = get_coordinates_latitude();
		uint32_t latitude_ns = htonl(latitude);
		memcpy(message_pointer, &latitude_ns, sizeof(latitude_ns));
		message_pointer += sizeof(latitude_ns);
		message_size += sizeof(latitude_ns);

		longitude = get_coordinates_longitude();
		uint32_t longitude_ns = htonl(longitude);
		memcpy(message_pointer, &longitude_ns, sizeof(longitude_ns));
		message_size += sizeof(longitude_ns);
	}	

	else if(message_type == 'P'){
		uint32_t search_radius, latitude_c, longitude_c;
		printf("Insira o raio de busca: ");
		
		scanf("%u", &search_radius);
		uint32_t search_radius_ns = htonl(search_radius);
		memcpy(message_pointer, &search_radius_ns, sizeof(search_radius_ns));
		message_pointer += sizeof(search_radius_ns);
		message_size += sizeof(search_radius_ns);

		printf("Digite a latitude e longitude do centro da busca\n");

		latitude_c = get_coordinates_latitude();
		uint32_t latitude_ns = htonl(latitude_c);
		memcpy(message_pointer, &latitude_ns, sizeof(latitude_ns));
		message_pointer += sizeof(latitude_ns);
		message_size += sizeof(latitude_ns);

		longitude_c = get_coordinates_longitude();
		uint32_t longitude_ns = htonl(longitude_c);
		memcpy(message_pointer, &longitude_ns, sizeof(longitude_ns));
		message_size += sizeof(longitude_ns);
	}
	else{
		fprintf(stderr, "Tipo de mensagem invalido\n");
		exit(EXIT_FAILURE);
	}
	uint32_t message_size_ns = htonl(message_size);
	memcpy(message, &message_size_ns, sizeof(message_size_ns));
}


