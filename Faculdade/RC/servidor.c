#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>

#define PORTA 8080

int main(int argc, char* argv[]){

	int servidor_fd, novo_socket;
	ssize_t valor_lido;
	struct sockaddr_in address;
	int opt = 1;
	socklen_t addrlen = sizeof(address);
	char buffer[1024] = {0};
	char* oi = "Bom dia do Servidor";

	if((servidor_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("Falha no socket");
		exit(EXIT_FAILURE);	
	}

	if(setsockopt(servidor_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
		perror("Problema no setsockopt");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORTA);

	if(bind(servidor_fd, (struct sockaddr*)&address, addrlen)){
		perror("Falha no bind");
		exit(EXIT_FAILURE);
	}

	if(listen(servidor_fd, 3) < 0){
		perror("Falha no listen");
		exit(EXIT_FAILURE);
	}

	if((novo_socket = accept(servidor_fd, (struct sockaddr*)&address, &addrlen)) < 0){
		perror("Erro no accept");
		exit(EXIT_FAILURE);
	}

	valor_lido = read(novo_socket, buffer, 1024 - 1);

	printf("%s\n", buffer);
	send(novo_socket, oi, strlen(oi), 0);
	printf("Mensagem Enviada do servidor\n");

	close(novo_socket);
	close(servidor_fd);
	
	return 0;



}

