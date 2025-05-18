#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

#define INT_STANDARD_SIZE sizeof(int32_t)  // tamanho de um inteiro em bytes
#define KEY 1337  // chave para memória compartilhada

typedef struct{
    int* startPos;   // ponteiro para início da fatia do array
    int* endPos;     // ponteiro para fim da fatia do array
    int* position;   // ponteiro para posição na memória compartilhada
} ProcessArgs;

// abre arquivo binário e armazena inteiros em numArray
void openFileAndStore(const char* fileName, int* arraySize, int** numArray) {
    struct stat fileStat;
    FILE* binFile;

    if ((binFile = fopen(fileName, "rb"))) {  // abre em modo binário
        int file_dcp = fileno(binFile);         // descritor do arquivo
        fstat(file_dcp, &fileStat);             // obtém tamanho
        *numArray = malloc(fileStat.st_size);   // aloca memória
        *arraySize = fileStat.st_size / INT_STANDARD_SIZE;  // conta elementos

        if (!*numArray) {    // verifica alocação
            fprintf(stderr, "Erro ao alocar memória em NumArray\n");
            exit(EXIT_FAILURE);
        }
        fread(*numArray, INT_STANDARD_SIZE, *arraySize, binFile);  // lê dados
        fclose(binFile);  // fecha arquivo

    } else {  // falha ao abrir
        fprintf(stderr, "Erro ao abrir o arquivo: %s\n", fileName);
        exit(EXIT_FAILURE);
    }
}

// soma valores entre startPos e endPos e armazena em position
void sumRoutine(ProcessArgs* args) {
    int* iterator = args->startPos;     // ponteiro de iteração
    *(args->position) = 0;              // zera posição compartilhada
    while (iterator != args->endPos) {  // percorre fatia do array
        *(args->position) += *iterator;  // acumula valor
        iterator++;                      // avança
    }
}

// cria processos para somar partes do array com memória compartilhada
void assignProcess(unsigned processQuant, int arraySize, int* totalSum, int** numberArray) {
    int shmid;
    int* shm;

    // cria segmento de memória compartilhada
    if ((shmid = shmget(IPC_PRIVATE, INT_STANDARD_SIZE * processQuant, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // anexa memória compartilhada
    if ((shm = shmat(shmid, NULL, 0)) == (int*)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    if (processQuant > (size_t)arraySize) {  // não mais processos que elementos
        processQuant = arraySize;
    }
    size_t arrayPortion = arraySize / processQuant;  // elementos por processo
    size_t remainder = arraySize % processQuant;     // resto da divisão
    ProcessArgs* args = malloc(sizeof(ProcessArgs) * processQuant);  // args de cada processo

    for (size_t count = 0; count < processQuant; count++) {
        if (fork() == 0) {  // processo filho
            size_t startPos = count * arrayPortion + (count < remainder ? count : remainder);  // início
            size_t elements = arrayPortion + (count < remainder ? 1 : 0);  // quantidade
            size_t endPos = startPos + elements;  // fim
            args[count].startPos = &(*numberArray)[startPos];  // define início
            args[count].endPos = &(*numberArray)[endPos];      // define fim
            args[count].position = &shm[count];                // posição na SHM
            sumRoutine(&args[count]);  // executa soma
            shmdt(shm);               // desanexa SHM
            exit(EXIT_SUCCESS);       // termina filho
        }
    }

    // espera todos os filhos
    for (size_t i = 0; i < processQuant; i++) {
        wait(NULL);
    }

    // agrega somas parciais
    for (size_t i = 0; i < processQuant; i++) {
        *totalSum += shm[i];
    }

    shmdt(shm);                     // desanexa SHM no pai
    shmctl(shmid, IPC_RMID, NULL);  // remove segmento
    free(args);                     // libera args
}

int main(int argc, char* argv[]) {
    char* fileName;
    int arraySize, *numberArray, totalSum;
    unsigned processQuant;

    totalSum = 0;  // inicializa soma

   
    if (argc != 3) {  // valida parâmetros
        fprintf(stderr, "Quantidade de parametros inválida\n");
        exit(EXIT_FAILURE);
    }

	fileName = argv[1];              // nome do arquivo
    processQuant = atoi(argv[2]);    // número de processos


    if (processQuant - 1 == 0) {  // valida processos
        fprintf(stderr, "Quantidade de processos invalida\n");
        exit(EXIT_FAILURE);
    }

    openFileAndStore(fileName, &arraySize, &numberArray);              // lê dados
    assignProcess(processQuant, arraySize, &totalSum, &numberArray);  // soma com processos

    printf("%d\n", totalSum);  // exibe resultado

    if (numberArray) {
        free(numberArray);  // libera array
    }

    return 0;  // fim do programa
}

