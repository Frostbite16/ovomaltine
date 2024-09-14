#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_PALAVRA 20
#define NO_TRANSICAO -1


typedef struct array{
	char** data;
	size_t size;
}array;

typedef struct transition{
	int* states;
	size_t size;
} transition;

void initialize(array* elemArray){
	elemArray->size = 0;
	elemArray->data = NULL;
}

void initializeTransition(transition* trans){
	trans->size = 0;
	trans->states = NULL;
}

void novoElem(array* elemArray, const char* palavra){
	char** array = elemArray->data;
	if(elemArray->size==0){
		elemArray->data = malloc(sizeof(char *)*(elemArray->size+1));
		elemArray->data[0] = (char*)(malloc(strlen(palavra)+1)); 
		strcpy(elemArray->data[0], palavra);
		elemArray->size++;
	}
	else{
		elemArray->data = realloc(elemArray->data, sizeof(char *)*(elemArray->size+1));
		elemArray->data[elemArray->size] = (char *)(malloc(strlen(palavra)+1));
		strcpy(elemArray->data[elemArray->size], palavra);
		elemArray->size++;
	}
}

void freeArray(array* elemArray){
	if((elemArray!=NULL)&&(elemArray->data!=NULL)){
		for(int i=0; i<elemArray->size; i++){
			if(elemArray->data[i] != NULL)
				free(elemArray->data[i]);
		}
		free(elemArray->data);
	}
}

void freeTransitionTable(transition** table, size_t numStates, size_t numSymbols){
	for(size_t i=0; i<numStates; i++){
		for(size_t j=0; j<numSymbols; j++){
			free(table[i][j].states);
		}
		free(table[i]);
	}
	free(table);
}

void processArray(char word[MAX_PALAVRA]){ // Remove os espaços da string
	char buffer[MAX_PALAVRA], *token;
	strcpy(buffer, word);

	token = strtok(buffer, " ");
	strcpy(word, token);
	while(token!=NULL){
		token = strtok(NULL, " ");
		if(token!=NULL)
			strcat(word, token);
	}	
	
}

void wordToArray(char word[MAX_PALAVRA], array *bDados){
	char* token = strtok(word, ",");
	while(token!=NULL){
		novoElem(bDados, token);
		token = strtok(NULL, ",");
	}
}

int searchArray(array *bDados, const char entrada[MAX_PALAVRA]);

void wordToArraySearched(char word[MAX_PALAVRA], array* src, array* dest){
	char* token = strtok(word, ",");
	while(token!=NULL){
		if(searchArray(dest, token)!=-1)
			novoElem(src, token);
		token = strtok(NULL, ",");
	}
}

int searchArray(array *bDados, const char entrada[MAX_PALAVRA]){
	size_t j=0, i=0;
	while(bDados->size>i){
		if(strcmp(bDados->data[i], entrada)==0)
			return i;
		i++;
	}	
	return -1;
}

void addTransition(transition* trans, int stateIndex){
	if(trans->size==0){
		trans->states = malloc(sizeof(int));
	}
	else{
		trans->states = realloc(trans->states, (trans->size + 1) * sizeof(int));
	}
	trans->states[trans->size] = stateIndex;
	trans->size++;
}

void createTransitionTable(transition** transitionTable, array* estados, array* alfabeto){
	char entrada[MAX_PALAVRA], *token;
	int estadoAtual;
	short unsigned check=0;
	for(size_t i = 0 ;i<estados->size; i++){
		for(size_t j = 0; j<alfabeto->size; j++){
			while(getchar()!='\n');

			printf("Transição do estado %s com o simbolo %s\n", estados->data[i], alfabeto->data[j]);
			printf("OBS: Separe mais de um estado por virgula, ou um estado fora do alfabeto para transição Vazia\n");
			scanf("%s", entrada);
			processArray(entrada);

			token = strtok(entrada, ",");
			check=0;
			while(token != NULL){
				estadoAtual = searchArray(estados, token);
				if(estadoAtual != -1){
					addTransition(&transitionTable[i][j], estadoAtual); 
					check=1;
				}
				else{
					printf("Estado %s invalido, ignorando\n", token);
				}
				token = strtok(NULL, ",");
			}
			if(check==0){
				addTransition(&transitionTable[i][j], -1);
			}
		}
	}
}


int main(){
	array estados, alfabeto, estadosFinais;
	initialize(&alfabeto);
	initialize(&estadosFinais);
	initialize(&estados);

	size_t esc=0, clock, check=0;
	short unsigned epsilon;
	char entrada[MAX_PALAVRA], estadoInicial[MAX_PALAVRA];
	transition** transitionTable;

	do{
		switch(esc){
			case 1:
				printf("Inserir estados, Não é permitido o simbolo '-'\n");
				printf("Digite somente um simbolo separado por virgula: ");
				while(getchar()!='\n');
				
				scanf("%s", entrada);
				
				processArray(entrada);
				wordToArray(entrada, &estados);

				printf("Inserir alfabeto\n");
				printf("Digite somente um simbolo separado por virgula: ");
				while(getchar()!='\n');
				
				scanf("%s", entrada);
				
				processArray(entrada);
				wordToArray(entrada, &alfabeto);

				printf("Digite os Estados Finais, separados, por virgula: ");
				while(getchar()!='\n');
				scanf("%s", entrada);

				processArray(entrada);
				wordToArraySearched(entrada, &estadosFinais, &estados);

				printf("Digite o estado inicial: ");
				while(getchar()!='\n');
				while(check!=1){
					scanf("%s", entrada);
					if(searchArray(&estados, entrada)!=-1){
						check=1;
						strcpy(estadoInicial, entrada);
					}
					else{
						printf("Estado não encontrado, Digite novamente: ");
						check=0;
					}
				}
				check=0;

				printf("Deseja adicionar um epsilon ao automato [1] sim, [0] não\n");
				while(getchar()!='\n');
				scanf("%lu", &esc);
				while(esc==1){
					while(getchar()!='\n');
					printf("Insira um simbolo que não está no alfabto: ");
					scanf("%s",entrada);
					if(searchArray(&alfabeto, entrada)!=-1){
						esc=1;
					}
					else{
						esc=0;
						novoElem(&alfabeto, entrada);
						epsilon = 1;
					}
				}		

				transitionTable = (transition**)malloc(estados.size*sizeof(transition*));
				for(size_t i=0; i<estados.size;i++){
					transitionTable[i] = (transition*)malloc(alfabeto.size * sizeof(transition));
					for(size_t j = 0; j<alfabeto.size; j++){
						initializeTransition(&transitionTable[i][j]);
					}
				}
				createTransitionTable(transitionTable, &estados, &alfabeto);
				

		}
		printf("[1] Criar AFN\n");
		scanf("%lu", &esc);
	}while(esc!=0);
	
	freeTransitionTable(transitionTable, estados.size, alfabeto.size);
	freeArray(&alfabeto);
	freeArray(&estados);
	freeArray(&estadosFinais);

}
