#include <stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>


#define PORTA 55000

struct gs_data_protocol_t{
	char message_type;
	int32_t message_ID;
	uint8_t fuel_type;
	int16_t fuel_price; // Multiplicado por 1000;
	uint32_t latitude; // (primeiro bit) N ou S, 8 bits - grau, mantissa - minutos e segundos (big endian) 
	uint32_t longitude; // (primeiro bit) W ou E, 8 bits - grau, mantissa - minutos e segundos (big endian)
};

struct gs_search_protocol_t{
	char message_type;
	int32_t message_ID;
	int32_t radius;	
	uint32_t latitude;
	uint32_t longitude;
};

int main(){
	int servidor_fd, cliente_fd;
	struct sockaddr_in address;
	socklen_t addrlen;
	int opt = 1;

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(PORTA);
	addrlen = sizeof(address);

	if((servidor_fd = socket(PF_INET, SOCK_DGRAM, 0))<0){
		perror("socket");
		exit(EXIT_FAILURE);
	}

	if(setsockopt(servidor_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	if((bind(servidor_fd, (struct sockaddr*)&address, addrlen) < 0)){
		perror("bind");
		exit(EXIT_FAILURE);
	}

	/*if(listen(servidor_fd, 3) == -1){
		perror("listen");
		exit(EXIT_FAILURE);
	}	

	if((cliente_fd = accept(servidor_fd, (struct sockaddr*)&address, &addrlen)) == -1){
		perror("accept");
		exit(EXIT_FAILURE);
	}*/
	
	char received_message[8192] = {0};
	int bytes_received;
	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);

	if((bytes_received = recvfrom(servidor_fd, received_message, 8192, 0, (struct sockaddr *)&client_addr, &client_addr_len)) == -1){
		perror("recvfrom");
		exit(EXIT_FAILURE);
	}

	printf("%d", bytes_received);

	close(servidor_fd);
}

