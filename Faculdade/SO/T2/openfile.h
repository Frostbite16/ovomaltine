#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>

#define INT_STANDARD_SIZE int32_t

void openFileAndStore(const char* fileName, int* arraySize, int** numArray){ // Open file with "Filename" name and store it in "NumArray" with the number of integers in file store in arraysize
	struct stat fileStat;
	FILE* binfile;
	int file_dcp;

	binfile = fopen(fileName, "rb"); // Open file in Read binary mode
	if(binfile){ // Check if the program succeded in opening the file
		file_dcp = fileno(binfile); // Get the descriptor of the file with the file pointer binfile	

		fstat(file_dcp, &fileStat); // Obtain the stats of the file
		*numArray = (int*)malloc(fileStat.st_size); // Allocate the necessary size in numArray
		*arraySize = fileStat.st_size / sizeof(INT_STANDARD_SIZE); // Save the quantity of integers in arraySize;

		if(!*numArray){ // Check if malloc successfully allocated the array, in case it dit not end the program with exit
			fprintf(stderr, "Erro ao alocar memória\n");
			exit(EXIT_FAILURE);
		}

		fread(*numArray, sizeof(INT_STANDARD_SIZE), *arraySize, binfile); // Read the whole content of "filename" and stores it in numArrat
		fclose(binfile);

	}
	else{ // In case it did not, it stops the program with exit
		fprintf(stderr, "Erro ao abrir o arquivo: %s\n", fileName);
		exit(EXIT_FAILURE);
	}
}
