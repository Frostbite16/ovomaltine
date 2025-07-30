#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<time.h>
#include <unistd.h>

#include "client_utils.h"
#include "generic_context.h"

int main(int argc, char* argv[]) {
    int sock_fd;
    struct sockaddr_in server_address;
    socklen_t addrlen = sizeof(server_address);

	if(argc > 4 || argc < 3){
		fprintf(stderr, "Número de argumentos incompativel\n");
		exit(EXIT_FAILURE);
	}

	int porta = atoi(argv[2]);

	if(porta < 1024){
		fprintf(stderr, "Número da porta deve exceder 1023\n");
		exit(EXIT_FAILURE);
	}


    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(porta);

    if(inet_pton(AF_INET, argv[1], &server_address.sin_addr)<0){
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    if((sock_fd = socket(AF_INET, SOCK_DGRAM, 0))<0){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct gs_data_protocol_t n_data_protocol;
    struct gs_search_protocol_t n_search_protocol;
    struct gs_generic_protocol_t new_message;

    new_message.message_ID = 0;
    new_message.data_protocol = &n_data_protocol;
    new_message.search_protocol = &n_search_protocol;

    char message_str[EXPECTED_MESSAGE_SIZE];

    get_message(&new_message);

    pack_message(&new_message, message_str);

    char message_rdt[EXPECTED_MESSAGE_SIZE + sizeof(uint32_t)];
    char* message_rdt_pt = message_rdt;

    enum rdt_check rdt_rec_check = NACK;
    uint16_t checksum;

    unsigned int seed = time(0);
    while(rdt_rec_check == NACK) {
        int rand_chance = rand_r(&seed) % 100;

        if (rand_chance > 50){
			checksum = 1;
		}
        else checksum = 0;
		
        checksum += get_checksum(message_str, EXPECTED_MESSAGE_SIZE);
		checksum = htons(checksum);

        message_rdt_pt = message_rdt;
        memcpy(message_rdt_pt, &checksum, sizeof(checksum));
        message_rdt_pt += sizeof(checksum);

        memcpy(message_rdt_pt, message_str, EXPECTED_MESSAGE_SIZE);
        sendto(sock_fd, message_rdt, sizeof(message_rdt), 0, (struct sockaddr *)&server_address, addrlen);
		
        if (recvfrom(sock_fd, &rdt_rec_check, sizeof(rdt_rec_check), 0, (struct sockaddr * )& server_address, &addrlen) == -1) {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }

		if(rdt_rec_check == NACK) printf("[ERRO] Dados corrompidos na jornada, enviando novamente...\n");
		else printf("[MENSAGEM] Confirmação de mensagem recebida\n");
    }

	char return_message[1024];
	if (recvfrom(sock_fd, return_message, sizeof(return_message), 0, (struct sockaddr * )& server_address, &addrlen) == -1) {
		perror("recvfrom");
		exit(EXIT_FAILURE);
	}
	
	printf("%s\n", return_message);

    close(sock_fd);

}
