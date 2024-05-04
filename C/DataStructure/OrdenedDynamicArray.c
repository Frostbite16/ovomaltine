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

void arrayRemove(array array, int x){
	unsigned i,j;

	for(i=0;i<=array.size;i++){
		if(array.end[i]==x){
			array.end[i]=0;
			for(j=i+1;j<=array.size;i++){
				array.end[j] = array.end[j-1];
			}
			array.size--;
			array.end = realloc(array.end, sizeof(int) * array.size);
		}
	}
}

int main(){
	array a1;
	arrayInnit(&a1);
	arrayInsert(&a1,6);
	arrayInsert(&a1,4);
	arrayInsert(&a1,5);
	for(unsigned i=0; i<=a1.size-1;i++){
		printf("%d ",a1.end[i]);
	}
	//arrayRemove(a1,5);
	return 0;


}

