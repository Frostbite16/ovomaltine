#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void allocateMatrix(size_t size, float** ptr){
	if((size>0)&&(size<=10))
		*(ptr) = malloc((size*(size)+1) * sizeof(float));

	for(size_t i=0;i<size*size;i++){
		(*ptr)[i] = i + 1;
	}
}

void matrixGauss(float* ptr, size_t size){
	float *aux = malloc(size*sizeof(float)), deter=1;
	size_t big=0, toplim=0, sidelim=0;
	for(size_t i=sidelim; i<size+1; i++){
		for(size_t j=toplim; j<size; j++){
			if(fabsf(ptr[j*size+i])>fabsf(ptr[big*size+i]))
				big = j;
		}
	}
}

int main(){
	float *ptr;
	size_t size = 3;
	allocateMatrix(size, &ptr);
	for(size_t i=0; i< size; i++){
		for(size_t j=0; j<size; j++){
			printf("%f",ptr[i*size + j]);
			if(j==size-1)
				printf("\n");

		}
	}

	return 1;
}
