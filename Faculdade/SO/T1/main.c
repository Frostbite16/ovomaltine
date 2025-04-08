#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<pwd.h>

#define PROC_DIR "/proc/"
#define SHELL_LINE "teste@teste: "
#define ARG_MAX 10

void commandReader(char* command){
	char *tokens = strtok(command, " "), *primeCommand, **arguments;
	short unsigned argMax = ARG_MAX, clock = 0;
	
	arguments = malloc(sizeof(char*)*ARG_MAX);
	primeCommand = tokens;
	tokens = strtok(NULL, " ");

	for(int i=0; i<ARG_MAX; i++){
		//arguments[i] = malloc(sizeof(char)*256);
		arguments[i] = NULL;
	}

	while(tokens != NULL){	
		strncpy(arguments[clock], tokens, strlen(tokens)+1);
		clock++;
	}

	for(int i=0; i<ARG_MAX; i++){
		if(arguments[i]){
			printf("%s", arguments[i]);
		}
	}

	execlp(primeCommand, primeCommand, arguments[0], arguments[1], arguments[2], NULL);

	
	free(arguments);
}

int main(){
	char commandLine[256];

	printf(SHELL_LINE);

	while(1){
		scanf("%s", commandLine);
		
		commandReader(commandLine);
	}	

	

	return 0;
}



