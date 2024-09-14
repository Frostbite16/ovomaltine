#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_PALAVRA 20


typedef struct array{
	char** data;
	size_t size;
}array;

void initialize(array* elemArray){
	elemArray->size = 0;
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
	}
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
	int encontrou;
	size_t j=0, i=0;
	while((encontrou!=1)&&(bDados->size>i)){
		encontrou = 0;
		if(strlen(bDados->data[i]) != strlen(entrada))
			encontrou = -1;
		while((encontrou!=-1)&&(bDados->data[i][j] != '\0')){
			if(bDados->data[i][j] == entrada[j]){
				encontrou = 1;
			}
			else{
				encontrou = -1;
			}
			j++;
		}
		j=0, i++;
	}	
	if(encontrou==1){
		return i;
	}
	return -1;
}


int main(){
	array estados, alfabeto, estadosFinais;
	initialize(&alfabeto);
	initialize(&estadosFinais);
	initialize(&estados);

	size_t esc=0, clock, check=0;
	short unsigned epsilon;
	char entrada[MAX_PALAVRA], estadoInicial[MAX_PALAVRA];

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
				while(esc!=1){
					while(getchar()!='\n');
					printf("Insira um simbolo que não está no alfabto: ");
					scanf("%s",entrada);
					if(searchArray(&alfabeto, entrada)==-1){
						esc=0;
					}
					else{
						esc=0;
						novoElem(&alfabeto, entrada);
					}
				}		


		}
		printf("[1] Criar AFN\n");
		scanf("%lu", &esc);
	}while(esc!=0);
	

	freeArray(&alfabeto);
	freeArray(&estados);
	freeArray(&estadosFinais);

}
