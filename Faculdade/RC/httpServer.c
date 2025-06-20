//
//
//          First try creating an http server in c
//
//
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>

#define PORTA 8080


struct http_response{
	char start_line[30];
	char* headers;
	char* empty_line;
	char* body;
};

void http_handling(struct http_response* response, char* request){

	char* request_status_line = strtok(request,"\r\n");

	if(request_status_line!=NULL && !strcmp(request_status_line, "GET / HTTP/1.1")){
		char status_line[] = "HTTP/1.1 200 OK\r\n";
		memcpy(response->start_line, status_line, strlen(status_line));
	}
	else{
		char status_line[] = "HTTP/1.1 404 NOT FOUND\r\n";
		memcpy(response->start_line, status_line, strlen(status_line));
	}
}

int main(){
	int servidor_fd, novo_socket;
	ssize_t valor_lido;
	struct sockaddr_in address;
	int opt = 1;
	socklen_t addrlen = sizeof(address);
	char buffer[8192] = {0};

	if((servidor_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("Falha no socket");
		exit(EXIT_FAILURE);
	}

	if(setsockopt(servidor_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
		perror("Problema no setsockopt");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_port = htons(PORTA);
	address.sin_addr.s_addr = INADDR_ANY;

	if(bind(servidor_fd, (struct sockaddr*)&address, addrlen)){
		perror("Falha no bind");
		exit(EXIT_FAILURE);
	}

	while(1){
		if(listen(servidor_fd, 3) < 0){
			perror("Falha no listem");
			exit(EXIT_FAILURE);
		}

		if((novo_socket = accept(servidor_fd, (struct sockaddr*)&address, &addrlen)) < 0){
			perror("Erro no accept");
			exit(EXIT_FAILURE);
		}

		valor_lido = read(novo_socket, buffer, 8192 - 1);

		struct http_response response;
		char empty_body[] = "ABCDEF";
		char empty_line[] = "\r\n";
		char header[] = "Date: Sat, 7 jul 2025 17:37 AMT\r\n"
						"Server: Apache/2.2.3\r\n"
						"Last-Modified: 7 jul 2025 17:37 AMT\r\n"
						"Content-Type: text/html\r\n"
						"Content-Lenght: 6\r\n";

		response.headers = header;
		response.body = empty_body;
		response.empty_line = empty_line;


		fprintf(stdout, "%s", buffer);

		http_handling(&response, buffer);

		size_t message_buffer_size = strlen(response.start_line) + strlen(response.headers) + strlen(response.empty_line) + strlen(response.body) + 1;
		char* message_buffer = calloc(1 ,sizeof(char) * message_buffer_size);
		strcat(message_buffer, response.start_line);
		strcat(message_buffer, response.headers);
		strcat(message_buffer, response.empty_line);
		strcat(message_buffer, response.body);

		send(novo_socket, message_buffer, message_buffer_size, MSG_CONFIRM);

		free(message_buffer);
		close(novo_socket);
	}
	
	close(servidor_fd);

}





