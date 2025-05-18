#include<stdio.h>
#include<stdlib.h>


void generateNumberFile(int size, int min, int max, const char* fileName){
	
	FILE* binfile;
	int randomNumber;

	binfile = fopen(fileName, "wb");

	for(int i=0; i<size; i++){
		randomNumber = (rand() % (max - min + 1)) + min;
		fwrite(&randomNumber, sizeof(int), 1, binfile);
	}	

	fclose(binfile);
	
}

int main(int argc, char* argv[]){

	size_t size, min, max;

	if(argc != 4){
		fprintf(stderr, "Quantidade de argumentos invalido");
		exit(EXIT_FAILURE);
	}
	
	size = atoi(argv[1]);
	min = atoi(argv[2]);
	max = atoi(argv[3]);

	generateNumberFile(size, min, max, argv[1]);

	


}



