#include<stdio.h>
#include<stdlib.h>

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

void insertDatainNewLine(array array, char x){
}

void CleanArray(array* array){
	free(array->data);
	initialize(array);
}



int main(){
	array estados, alfabeto, estadosFinais;
	char estadoInicial;
	initialize(&estados);
	initialize(&alfabeto);
	initialize(&estadosFinais);
	insertData(&estados, 'a');
	for(size_t i=0; i<estados.size; i++){
		printf("%c", estados.data[i]);
	}

	free(estados.data);
	free(alfabeto.data);
	free(estadosFinais.data);
}
