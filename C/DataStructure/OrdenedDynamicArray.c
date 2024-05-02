//Array dinamica ordenada de alocacação sequencial
// Alocar - feita
// Inserir - feita
// Remover
// Desalocar

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

void arrayInsert(array array, int x){
	int i,j;

	array.size++;	
	//array.end = realloc(array.end, sizeof(int) * array.size);
	for(i=0;i<=array.size-1;i++){
		if(i==array.size-1){
			array.end[i]=x;
		}
	
		if(array.end[i]>=x){
			for(j=array.size-1;j>=i;j--){
				array.end[j+1] = array.end[j];
			
			}
			array.end[i] = x;
			i = array.size;	

		}
  }
}

