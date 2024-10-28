#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define NUM_STATES 10
#define NUM_ALPHABET 10

typedef struct array{
	char* data; 
	size_t size;
	struct array* prox;
}array;

void initialize(array* array){
	array->data = malloc(sizeof(char));
	array->size = 0;
}

void insertData(array* array, char x){
	if(array->size==0){
		array->data[0] = x;
		array->size++;
	}
	else{
		array->data = realloc(array->data,sizeof(char)*array->size);
		array->data[array->size] = x;
		array->size++;
	}
}
void CleanArray(array* array){
	free(array->data);
	initialize(array);
}

void initializeMatrix(char* matrix[NUM_STATES][NUM_ALPHABET]){ //Inutil
	for(int i=0; i<=NUM_STATES; i++){
		for(int j=0; j<=NUM_ALPHABET; j++){
			matrix[i][j] = malloc(sizeof(char));
		}
	}
}

void freeMatrix(char*  matrix[NUM_STATES][NUM_ALPHABET]){ // possivelmente inutil
	for(size_t i=0; i<=NUM_STATES; i++){
		for(size_t j=0; j<=NUM_ALPHABET; j++){
			free(matrix[i][j]);
		}
	}
}

size_t searchArray(array* array, char x){
	for(size_t i=0; i<array->size; i++){
		if(*(array->data + i)==x){
			return i;
		}
	}	
	return NUM_STATES + NUM_ALPHABET + 1;
}


void cleanMatrix(char* matrix[NUM_STATES][NUM_ALPHABET]){
	for(int i=0; i<NUM_STATES; i++){
		for(int j=0; j<NUM_ALPHABET; j++){
			strcpy(matrix[i][j], "\0");
		}
	}

}


void guardarFuncTransicao(char* matrix[NUM_STATES][NUM_ALPHABET], size_t indexStates, size_t indexAlphabet, char* nextStates){
	matrix[indexStates][indexAlphabet] = nextStates;
}
void mudarEstado(char entrada, char estadoAtual[50], char* matrix[NUM_STATES][NUM_ALPHABET], array* estados, array* alfabeto){
	size_t i=0, indexStates, indexEntrada, tamEstadoTotal = 0;
	char buffer[50] = {0x0};
	indexEntrada = searchArray(alfabeto, entrada);
	while(estadoAtual[i] != '\0'){
		indexStates = searchArray(estados, estadoAtual[i]);
		tamEstadoTotal += strlen(matrix[indexStates][indexEntrada]);
		if(matrix[indexStates][indexEntrada][0]!='-'){
			strcat(buffer, matrix[indexStates][indexEntrada]);
		}
		i++;
	}

	memcpy(estadoAtual, buffer, tamEstadoTotal);
	
}
	
int main(){
	array estados, alfabeto, estadosFinais;
	char estadoInicial, *tabelaTransic[NUM_STATES][NUM_ALPHABET], estadoAtual[50] = "a";
	initialize(&estados);
	initialize(&alfabeto);
	initialize(&estadosFinais);

	size_t esc=0, clock, check, epsilon;
	char data, entrada[50], buffer[50]={0x0};

	do{
		switch(esc){
			case 1:
				printf("Inserir estados, Não é permitido o simbolo '-'\n");	
				printf("Digite somente um simbolo separado por virgula: ");
				while(getchar()!='\n');
				scanf("%s", entrada);
				for(size_t i=0; entrada[i]!='\0'; i++){
					if((entrada[i])!='-'&&(entrada[i]!=',')&&(entrada[i]!=' '))
						insertData(&estados, entrada[i]);
				}
				entrada[0] = '\0';
				printf("Inserir alfabeto\n");
				printf("Digite somente um simbolo separado por virgula: ");
				while(getchar()!='\n');
				scanf("%s", entrada);
				for(size_t i=0; entrada[i]!='\0'; i++){
					if((entrada[i]!=',')&&(entrada[i]!=' '))
						insertData(&alfabeto, entrada[i]);
				}
				entrada[0] = '\0';

				printf("Digite os Estados Finais, separados por virgula: ");
				while(getchar()!='\n');
				scanf("%s", entrada);
				for(size_t i=0; entrada[i]!='\0'; i++){
					if((entrada[i] != ',')&&(searchArray(&estados, entrada[i])!=NUM_STATES+NUM_ALPHABET+1)){
						insertData(&estadosFinais, entrada[i]);
					}
				}
				entrada[0] = '\0';
				esc = NUM_STATES + NUM_ALPHABET + 1;
				printf("Digite o estado inicial: ");
				while(esc == NUM_STATES + NUM_ALPHABET + 1){
					while(getchar()!='\n');
					scanf("%c",&data);
					if((esc=searchArray(&estados, data))!=NUM_ALPHABET + NUM_ALPHABET + 1)
						estadoInicial = data;
				}

				printf("Deseja adicionar um epsilon ao automato [1] sim [0] não: ");
				while(getchar()!='\n');
				scanf("%lu", &epsilon);
				while(esc!=1){
					while(getchar()!='\n');
					printf("Insira um simbolo que não está no alfabeto: ");
					scanf("%c",&data);
					if(searchArray(&alfabeto, data)!=NUM_STATES+NUM_ALPHABET+1){
						esc=1;
						printf("O simbolo não deve estar no alfabeto\n");
					}
					else{
						esc=0;
						insertData(&alfabeto, data);
					}
				}

				// Matrix de transição
				printf("Montar a matrix de transicao\n");
				printf("Digite cada estado separado por virgula\n");
				for(size_t i=0; i<estados.size; i++){	
					for(size_t j=0; j<alfabeto.size; j++){
						buffer[0] = '\0';
						if((epsilon!=1)||(j<alfabeto.size)){
							printf("Do estado %c com a entrada %c: \n", estados.data[i], alfabeto.data[j]);
							printf("Digite '-' ou qualquer outro simbolo fora para um transição invalida: ");
							while(getchar()!='\n');
							scanf("%s",entrada);
							esc=0;
						}
						else{
							printf("Transicao epsilon do estado %c? [1] sim [0] não: ", estados.data[i]);
							while(getchar()!='\n');
							scanf("%lu",&esc);
						}
						if(((esc==1)&&(epsilon==1)&&(j=alfabeto.size))||(epsilon!=1)){
							if(esc==1){
								printf("Digite os estados destino do estado %c com transição epsilon", estados.data[i]);
								while(getchar()!='\n');
								scanf("%s",entrada);
							}
							for(int k=0; entrada[k]!='\0'; k++){
								if((entrada[k]!='-')&&(entrada[k]!=',')&&((check = searchArray(&estados, entrada[k]) != NUM_STATES + NUM_ALPHABET + 1))){
									clock++;	
									strcat(buffer, &entrada[k]);
								}
								else if(check == (NUM_STATES + NUM_ALPHABET + 1)){
									entrada[k+1] = '\0';
									buffer[1] = '\0';
									buffer[0] = '-';
								}
							}
						}	
						entrada[0] = '\0';
						//strcpy(tabelaTransic[i][j], buffer);
						for(int k=0; buffer[k-1] != '\0'; k++){
							tabelaTransic[i][j][k] = buffer[k];
						}
					}
				}	
				break;
			case 0:
				break;
			default:
				break;
		}
		
		printf("[1] Criar AFN\n");
		printf("[2] Converter AFD em AFN\n");
		printf("[3] Limpar Automato\n");
		printf("[4] Rodar Programa\n");
		printf("[0] Sair do programa\n");
		scanf("%lu", &esc);
	}while(esc!=0);

	free(estados.data);
	free(alfabeto.data);
	free(estadosFinais.data);
}
