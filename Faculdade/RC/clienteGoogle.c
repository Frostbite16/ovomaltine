#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netdb.h>
#include<unistd.h>


int main(){
	int servidor_fd;
	struct addrinfo address_hints, *res;

	memset(&address_hints, 0, sizeof(address_hints));

	address_hints.ai_family = AF_UNSPEC;
	address_hints.ai_socktype = SOCK_STREAM;

	if(getaddrinfo("www.google.com", "80", &address_hints, &res) != 0){
		perror("getaddrinfo");
		exit(EXIT_FAILURE);
	}


	if((servidor_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0){
		perror("socket");
		exit(EXIT_FAILURE);
	}
	
	char buffer[32];
	inet_ntop(res->ai_family, &res->ai_addr, buffer, res->ai_addrlen);
	printf("%s\n", buffer);

	if(connect(servidor_fd, res->ai_addr, res->ai_addrlen) < 0){
		perror("connect");
		exit(EXIT_FAILURE);
	}

	char request[] =	"GET / HTTP/1.1\r\nHost: www.google.com\r\n\r\n";
				
	send(servidor_fd, request, sizeof(request), 0);
	char response_buffer[8192];
	read(servidor_fd, response_buffer, 8192-1);
	printf("%s\n",response_buffer);

	close(servidor_fd);
	freeaddrinfo(res);

}
