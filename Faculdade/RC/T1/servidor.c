#define _GNU_SOURCE
#include <stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/types.h>
#include "server_utils.h"

struct thread_args {
	char* received_message;
	size_t message_size;
	struct sockaddr_in client_address;
	int client_socket;
};

void* thread_safe_receive_message(void* arg) {
	struct thread_args* args = (struct thread_args*)arg;
	
	printf("[MENSAGEM] Novo thread aberto: %d\n", gettid());

	receive_message(args->received_message, args->message_size, args->client_address, args->client_socket);
	free(args->received_message);
	free(args);
	return NULL;

}

int main(int argc, char* argv[]){
	int servidor_fd;
	struct sockaddr_in address;
	socklen_t addrlen;
	int opt = 1;

	if(argc > 3 || argc < 2){
		fprintf(stderr, "Número de argumentos incompativel\n");
		exit(EXIT_FAILURE);
	}

	int porta = atoi(argv[1]);

	if(porta < 1024){
		fprintf(stderr, "Número da porta deve exceder 1023");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(porta);
	addrlen = sizeof(address);
	
	// Criando input socket
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

	printf("[ROTINA] Servidor Iniciado\n");

	while(1) {
		char received_message[8192] = {0};
		char ipv4_str[INET_ADDRSTRLEN];
		int bytes_received;
		struct sockaddr_in client_addr;
		socklen_t client_addr_len = sizeof(client_addr);

		if((bytes_received = recvfrom(servidor_fd, received_message, 8192, 0, (struct sockaddr *)&client_addr, &client_addr_len)) == -1){
			perror("recvfrom");
			exit(EXIT_FAILURE);
		}
		

		if(inet_ntop(AF_INET, &client_addr.sin_addr, ipv4_str, INET_ADDRSTRLEN) != NULL)
			printf("[MENSAGEM] Mensagem recebida de %s\n", ipv4_str);
		else
			perror("inet_ntop");

		pthread_t thread_id;
		struct thread_args* thread_args = (struct thread_args*)malloc(sizeof(struct thread_args));
		thread_args->received_message = malloc(sizeof(char) * bytes_received);
	
		memcpy(thread_args->received_message, received_message, bytes_received);

		thread_args->message_size = bytes_received;
		thread_args->client_address = client_addr;
		thread_args->client_socket = servidor_fd;

		pthread_create(&thread_id, NULL, thread_safe_receive_message, (void*)thread_args);
		pthread_detach(thread_id);
		//receive_message(received_message, bytes_received, client_addr, servidor_fd);
	}
	close(servidor_fd);
}

