#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <limits.h>

#define THREADS_MAX 100000

#define INT_STANDARD_SIZE int32_t  // define o tipo padrão de inteiro

typedef struct ThreadArgs{
    int* startPos;    // ponteiro para início da fatia
    int* endPos;      // ponteiro para fim da fatia
    int  partialSum;  // soma parcial calculada pela thread
}ThreadArgs;

// abre o arquivo binário e armazena os inteiros em um array
void openFileAndStore(const char* fileName, int* arraySize, int** numArray){
    struct stat fileStat;
    FILE* binfile;

    binfile = fopen(fileName, "rb");  // abre em modo binário

    if(binfile){  // verifica abertura
        int file_dcp = fileno(binfile);  // obtém descritor do arquivo

        fstat(file_dcp, &fileStat);  // obtém informações do arquivo
        *numArray = (int*)malloc(fileStat.st_size);  // aloca memória
        *arraySize = fileStat.st_size / sizeof(INT_STANDARD_SIZE);  // conta elementos

        if(!*numArray){  // verifica alocação
            fprintf(stderr, "Erro ao alocar memória\n");
            exit(EXIT_FAILURE);
        }

        fread(*numArray, sizeof(INT_STANDARD_SIZE), *arraySize, binfile);  // lê dados
        fclose(binfile);  // fecha arquivo

    }
    else{  // erro ao abrir
        fprintf(stderr, "Erro ao abrir o arquivo: %s\n", fileName);
        exit(EXIT_FAILURE);
    }
}

// função que cada thread executa para somar parte do array
void* sumPortion(void* args){
    int* startPos = ((ThreadArgs*)args)->startPos;  // início
    int* endPos = ((ThreadArgs*)args)->endPos;      // fim
    int* partialSum = &((ThreadArgs*)args)->partialSum;  // soma parcial

    *partialSum = 0;    
    for(int* iterator=startPos; iterator!=endPos; iterator++){  // itera até o fim
        *partialSum += *iterator;  // acumula valor
    }
    return NULL; 
}

// cria threads e divide o trabalho
void assignThread(unsigned threadsQuant, int arraySize, int *totalSum, int** numberArray) {
    size_t arrayPortion, remainder;
    pthread_t* threads;
    ThreadArgs* args;

    threads = malloc(sizeof(pthread_t)*threadsQuant);  // aloca vetor de threads
    args = calloc(threadsQuant,sizeof(ThreadArgs));    // aloca argumentos

    arrayPortion = arraySize/threadsQuant;  // portion size
    remainder = arraySize % threadsQuant;    // resto da divisão

    for(size_t count=0; count<threadsQuant; count++){

        size_t startPos = count*arrayPortion + (count<remainder ? count:remainder);  // cálculo do início
        size_t elements = arrayPortion + (count<remainder ? 1:0);  // elementos extras
        size_t endPos = startPos + elements;  // fim da fatia
        args[count].startPos = &(*numberArray)[startPos];  // define início
        args[count].endPos = &(*numberArray)[endPos];      // define fim

		pthread_create(&threads[count], NULL, sumPortion, &args[count]);  // cria thread

	}

    for(size_t i=0; i<threadsQuant; i++){
        pthread_join(threads[i], NULL);  // aguarda término
        *totalSum += args[i].partialSum;  // adiciona soma parcial
    }
    free(threads);  // libera memória
    free(args);
}

int main(int argc, char* argv[]){
    char* fileName;
    int arraySize, *numberArray, totalSum;
    unsigned threadsQuant;

    totalSum = 0;  // inicializa soma total

    if(argc>3 || argc<3){  // verifica número de argumentos
        fprintf(stderr, "Numero de argumentos invalido\n");
        exit(EXIT_FAILURE);
    }

    fileName = argv[1];  // nome do arquivo
    threadsQuant = atoi(argv[2]);  // converte pra inteiro

    if(threadsQuant-1<=0 || threadsQuant > THREADS_MAX){  // threads deve ser >= 1
        fprintf(stderr, "Quantidade de threads invalida\n");
        exit(EXIT_FAILURE);
    }

    openFileAndStore(fileName, &arraySize, &numberArray);  // carrega dados
    assignThread(threadsQuant, arraySize, &totalSum, &numberArray);  // soma com threads
    
    printf("%d\n",totalSum);  // imprime resultado

    if(numberArray){  // libera array
        free(numberArray);
    }

    return 0;  // fim
}

