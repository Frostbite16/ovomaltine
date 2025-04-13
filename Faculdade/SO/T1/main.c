#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include<unistd.h>
#include<pwd.h>

#define PROC_DIR "/proc/"
#define SHELL_LINE "teste@teste: "
#define ARG_MAX 10


void commandReader(char* command){ /* Função para tratar o texto do comando e executar com execlp */
	char *tokens, *primeCommand, **arguments, **temp;
	short unsigned clock = 1, flag = 0; /* Declaração de variáveis */
	pid_t pid;
	int value_check;

	if(command[strlen(command)-1] == '&') { /* Parte da implementação de regra estabelecida que o emulador de terminal não espera até o final da execução do comando caso o comando termine com &*/
		command[strlen(command)-1] = '\0'; /* Remove o & do comando e ativa a flag */ 
		flag = 1;
	}

	tokens = strtok(command, " "); /* Divide o comando entre espaços */
	
	if(!strcmp(tokens, "exit")){
		printf("Fechando Terminal\n");
		exit(EXIT_SUCCESS);
	}

	primeCommand = tokens; /* Comando principal, Ex: primeCommand(whereis) arguments(primeCommand,arg1 arg2) */
	arguments = malloc(sizeof(char*));					 /* Esses argumentos são necessários para o comando inicial, eles não representam*/
	arguments[0] = malloc(sizeof(char)*strlen(tokens)+1);/* nenhum argumento */

	tokens = strtok(NULL, " ");

	while(tokens != NULL) {/* Copia cada argumento para a variavel arguments, caso não tenha argumentos clock será 0 */

		temp = realloc(arguments, sizeof(char*)*(clock+1));
		if(temp) {
			arguments = temp;
		}
		else{ /* Caso realloc falhe em alocar memória */
			for(int i=clock-1; i>=0; i--){
				free(arguments[i]);
			}
			free(arguments);
			fprintf(stderr, "Erro alocando memória.\n");
			exit(EXIT_FAILURE);
		}


		arguments[clock] = malloc(sizeof(char)*strlen(tokens)+1);
		strncpy(arguments[clock], tokens, strlen(tokens)+1);
		tokens = strtok(NULL, " ");
		clock++;
	}

	if (!((temp = realloc(arguments, sizeof(char*)*(clock+1))))){ /* Caso realloc falhe em alocar memória */
		for(int i=clock-1; i>=0; i--){
			free(arguments[i]);
		}
		free(arguments);
		fprintf(stderr, "Erro alocando memória.\n");
		exit(EXIT_FAILURE);
	}
	arguments = temp;

	arguments[clock] = NULL;
	strcpy(arguments[0], primeCommand);

	pid = fork();
	if(pid==0){
		value_check = execvp(primeCommand, arguments);
	}

	if(flag == 0) { /*Espera até o termino da execução do ultimo processo, ou seja, do ultimo algurmento da linha de comando*/
		waitpid(pid, NULL, 0);

	}

	if (value_check == -1) {
		printf("Erro na execucao do programa\n");
	}

	for(int i=0; i<clock; i++) { /* Libera os blocos usados*/
		free(arguments[i]);
	}
	
	free(arguments); /* Libera toda a matris alocada*/

}

int main(){
	char commandLine[256];

	while(1){
		printf(SHELL_LINE);
		fgets(commandLine, 256, stdin);

		commandLine[strlen(commandLine)-1] = '\0';
		commandReader(commandLine);

	}
	return 0;
}



