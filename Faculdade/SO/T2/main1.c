#include<pthread.h>
#include <stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include"openfile.h" /*Header com função para abrir arquivo e salvar em um vetor*/

typedef struct ThreadArgs{
	int* startPos;
	int* endPos;
	int* partialSum;
}ThreadArgs;

void* sumPortion(void* args){
	int* startPos = ((ThreadArgs*)args)->startPos;
	int* endPos = ((ThreadArgs*)args)->endPos;
	int *partialSum = ((ThreadArgs*)args)->partialSum;

	*partialSum = 0;	
	for(int* iterator=startPos; iterator!=endPos; iterator++){
		*partialSum += *iterator;
	}

	pthread_exit(partialSum);	

}

void assignThread(unsigned short threadsQuant, int arraySize, int *totalSum, int** numberArray){
	size_t arrayPortion;
	int *partialSums = malloc(sizeof(int)*threadsQuant);
	void* partialSum;
	
	threadsQuant--;
	pthread_t* threads = malloc(sizeof(pthread_t)*threadsQuant);


	if(threadsQuant>= arraySize)
		arrayPortion = 1;
	else
		arrayPortion = (int)arraySize/threadsQuant;
	if(arraySize%threadsQuant==0){
		for(size_t count=0;count<=arraySize-arrayPortion;count+=arrayPortion){
			ThreadArgs args;
			args.startPos = &(*numberArray)[count];
			args.endPos = &(*numberArray)[count+arrayPortion-1];
			args.partialSum = &partialSums[count];
			pthread_create(&threads[count],NULL, sumPortion, &args);
		}
	}
	for(int i=0; i<threadsQuant; i++){
		pthread_join(threads[i], &partialSum);
		*totalSum += *(int*)partialSum;
	}
	
	free(partialSums);
	free(threads);
}



int main(int argc, char* argv[]){
	char* fileName;
	int arraySize, *numberArray, totalSum;
	unsigned short threadsQuant;

	if(argc>3 || argc<3){
		//fprintf(stderr, "Numero de argumentos invalido\n");
		//exit(EXIT_FAILURE);
	}

	
	//fileName = argv[1];
	//threadsQuant = atoi(argv[2]);
	fileName = "100.a";
	threadsQuant = 2;

	if(threadsQuant-1==0){
		fprintf(stderr, "Quantidade de threads invalida\n");
		exit(EXIT_FAILURE);
	}

	openFileAndStore(fileName, &arraySize, &numberArray);
	assignThread(threadsQuant, arraySize, &totalSum, &numberArray);
	
	printf("%d aaaaaa",totalSum);

	if(numberArray){
		free(numberArray);
	}
	pthread_exit(NULL);
	return 1;
}
