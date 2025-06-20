#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>

#define PORTA 8080

int main(int argc, char* argv[]){
	int cliente_fd, valor_lido, status;
	struct sockaddr_in serv_addr;
	char* oi = "Bom dia do cliente";
	char buffer[1024] = {0};

	if((cliente_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("Erro no socket");
		exit(EXIT_FAILURE);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORTA);

	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) < 0){
		perror("Erro na conversao de endereços");
		exit(EXIT_FAILURE);
	}

	if((status = connect(cliente_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0){
		perror("Erro no connect");
		exit(EXIT_FAILURE);
	}

	send(cliente_fd, oi, strlen(oi), 0);
	printf("Mensagem enviada do cliente\n");
	valor_lido = read(cliente_fd, buffer, 1024-1);
	printf("%s\n",buffer);

	close(cliente_fd);
	return 0;

}
