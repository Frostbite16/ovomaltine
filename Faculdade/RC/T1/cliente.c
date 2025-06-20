#include <stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include"client_utils.h"
#include<string.h>

#define PORTA 55000

int main(){
	int servidor_fd;
	struct sockaddr_in address;
	socklen_t addrlen = sizeof(address);
	int status;
	char message[8192] = {0};
	
	if((servidor_fd = socket(AF_INET, SOCK_DGRAM, 0))<0){
	perror("socket");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_port = htons(PORTA);

	

	if(inet_pton(AF_INET, "127.0.0.1", &address.sin_addr)<0){
		perror("inet_pton");
		exit(EXIT_FAILURE);
	}
	
	if((status = connect(servidor_fd, (struct sockaddr*)&address, addrlen)) < 0){
		perror("connect");
		exit(EXIT_FAILURE);
	}

	get_input(message);
	

	sendto(servidor_fd, message, sizeof(message), 0, (struct sockaddr *)&address, addrlen);

	close(servidor_fd);
	return 0;

}
