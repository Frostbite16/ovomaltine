//Array dinamica ordenada de alocacação sequencial
// Alocar - feita
// Inserir - feita
// Remover - feita
// Desalocar - feita

#include <stdio.h>
#include <stdlib.h>


typedef struct array{
	int *end;
	int size;
}array;
 
void arrayInnit(array *array){
	array->end = malloc(sizeof(int));
	array->end[0] = 0;
	array->size = 0;
} 

void arrayInsert(array *array, int x){
	int i,j;
	array->size+=1;

	array->end = realloc(array->end, sizeof(int) * array->size);
	for(i=0;i<=array->size-1;i++){
	
		if(i==array->size-1){
			array->end[i]=x;
			i = array->size;
		}

		if(array->end[i]>=x){
			for(j=array->size-1;j>=i;j--){
				array->end[j+1] = array->end[j];
			}
			array->end[i] = x;
			i = array->size;	
		}
  	}
}

int *arrayBSearch(array *array, int x){
	int meio, inf=1, sup=array->size;
	while (inf<=sup){
		meio = (inf+sup)/2;
		if(array->end[meio]==x){
			return &array->end[meio];
		}
		else if (array->end[meio]<x){
			inf = meio + 1;
		}
		else{
			sup = meio - 1;
		}
	}
	return NULL;
}

void arrayRemove(array *array, int x){
	unsigned i,j;

	for(i=0;i<=array->size-1;i++){

		if(array->end[i]==x){

			for(j=i+1;j<=array->size-1;j++){
				array->end[j-1] = array->end[j];
			}
			array->size--;
			array->end = realloc(array->end, sizeof(int) * array->size);
		}
	}
}

void arrayDeallocate(array *array){
	printf("%p ",array);
	free(array);
	printf("%p",array);
}





