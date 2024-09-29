#ifdef __linux__
	#define clear "clear"
#elif WIN_32
	#define clear "cls"
#endif

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>


typedef struct numArray{
	int32_t *data;
	size_t size;
}numArray;

void trocar(int32_t* num1, int32_t* num2){
	*num1 += *num2;
	*num2 = *num1 - *num2;
	*num1 = *num1 - *num2;
}

void bubbleSort(numArray* buffer){
	for(size_t i=0; i<buffer->size; ++i){
		for(size_t j=0; j<buffer->size-1; ++j){
			if(buffer->data[j] > buffer->data[j+1]){
				trocar(&buffer->data[j], &buffer->data[j+1]);
			}

		}
	}
}

void bubbleSortImprov(numArray* buffer){
	unsigned short switched = 1;
	size_t index_aux = buffer->size-1, index_limit = buffer->size-1, j;

	while(switched){
		j = 0, switched = 0;
		while(j<index_limit){
			if(buffer->data[j] > buffer->data[j+1]){
				trocar(&buffer->data[j], &buffer->data[j+1]);
				switched = 1;
				index_aux = j;
			}
			j++;
		}
		index_limit = index_aux;
	}
}

void insertionSort(numArray* buffer){
	int32_t aux, key;
	size_t j;
	for(size_t i=1; i<buffer->size; ++i){
		aux = buffer->data[i], key = buffer->data[i];
		j = i-1;
		while((j>=1)&&(key<buffer->data[j])){
			buffer->data[j+1] = buffer->data[j];
			--j;
		}
		buffer->data[j+1] = aux;
	}
}


short int openFile(const char* fileName, int* file_dcp, FILE** binfile){
	*binfile = fopen(fileName, "rb");
	if(*binfile){
		*file_dcp = fileno(*binfile);
		return 1;
	}
	*file_dcp = -1;
	return 0;
}


void insertData(FILE** binfile, const char* fileName, numArray* buffer){
	struct stat fileStat;
	int file_dcp;

	openFile(fileName, &file_dcp, binfile);
	fstat(file_dcp, &fileStat);
	
	buffer->data = malloc(fileStat.st_size);
	buffer->size = fileStat.st_size/sizeof(int32_t);

	fread(buffer->data, sizeof(int32_t), fileStat.st_size/sizeof(int32_t), *binfile);

	fclose(*binfile);

}





int main(int argc, char* argv[]){
	FILE* binfile;
	numArray array;
	insertData(&binfile, argv[2], &array);
	short unsigned esc;
	
	
	if( argc != 3){
		printf("Formato: (Escolha de algoritmo) (Nome do arquivo)");
		return 0;
	}

	esc = atoi(argv[1]);

	switch(esc){
		case 1:
			bubbleSort(&array);
			break;
		
		case 2:
			bubbleSortImprov(&array);
			break;
		
		case 3:
			insertionSort(&array);
			break;

		default:
			printf("Opcao invalida, vetor nao ordenado\n");
			break;
	}
	

	for(size_t i=0; i<array.size; ++i){
		printf("%d ", array.data[i]);
	}

	free(array.data);
}


