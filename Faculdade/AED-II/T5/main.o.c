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
#include<time.h>
#include<string.h>


typedef struct numArray{
	int32_t *data;
	size_t size;
}numArray;

void trocar(int32_t* num1, int32_t* num2){
	if(*num1!=*num2){
		*num1 += *num2;
		*num2 = *num1 - *num2;
		*num1 = *num1 - *num2;
	}
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
	int j;
	for(size_t i=1; i<buffer->size; ++i){
		aux = buffer->data[i], key = buffer->data[i];
		j = i-1;
		while((j>=0)&&(key<buffer->data[j])){
			buffer->data[j+1] = buffer->data[j];
			--j;
		}
		buffer->data[j+1] = aux;
	}
}

void merge(numArray* buffer, size_t esq, size_t center, size_t dir){
	size_t fim1 = center-1, ind = 0, nro=0, start = esq;
	int32_t *tempBuffer;
	tempBuffer = malloc( (dir-esq + 1) * sizeof(int32_t));
	while(esq<=fim1 && center <= dir){
		if(buffer->data[esq] < buffer->data[center]){
			tempBuffer[ind] = buffer->data[esq];
			esq++;
		}
		else{
			tempBuffer[ind] = buffer->data[center];
			center++;
		}
		ind++, nro++;
	}
	while(esq<=fim1){
		tempBuffer[ind] = buffer->data[esq];
		esq++, ind++, nro++;
	}
	while(center<=dir){
		tempBuffer[ind] = buffer->data[center];
		center++, ind++, nro++;
	}
	for(size_t i=0; i<nro; ++i){
		buffer->data[start+i] = tempBuffer[i];
	}
	free(tempBuffer);
}

void mergeSort(numArray* buffer, int esq, int dir){
	if(esq<dir){
		mergeSort(buffer, esq,(esq+dir)/2);
		mergeSort(buffer, (esq+dir)/2+1, dir);
		merge(buffer, esq, (esq+dir)/2+1, dir); 
	}
}

void pivot(unsigned short esc, numArray* buffer, size_t ini ,size_t end, size_t *pivo){
	size_t meio;
	if(ini==end){
		*pivo=ini;
		return;
	}

	switch(esc){
		case 1: // pivo aleatorio
			*pivo = ini + rand() % (end-ini+1);
			break;
		case 2: // pivo como ultimo elemento
			*pivo = end;
			break;
		case 3: // pivo como mediana de três
			meio = (ini+end)/2;
			if(buffer->data[ini]>buffer->data[meio])
				trocar(&buffer->data[ini], &buffer->data[meio]);
			if(buffer->data[ini]>buffer->data[end])
				trocar(&buffer->data[ini], &buffer->data[end]);
			if(buffer->data[meio] > buffer->data[end]) 
				trocar(&buffer->data[meio], &buffer->data[end]);
			*pivo = meio;
			break;
		default:
			*pivo = ini;
			break;
	}

}

void quickSort(numArray* buffer, int ini, int end, unsigned short esc){
	size_t pivo;
	int i,j;
	int32_t auxKey;
	if(ini>=end)
		return;
	if(end-ini<=1){
		if(end-ini==1){
			if(buffer->data[ini]>buffer->data[end])
				trocar(&buffer->data[ini], &buffer->data[end]);
		}
		return;
	}
	

	pivot(esc, buffer, ini, end, &pivo);
	auxKey = buffer->data[pivo];
	trocar(&buffer->data[pivo], &buffer->data[end]);

	i=ini; 
	j=end-1;
	while(i <= j){
		while((buffer->data[i] < auxKey) && (i <= j)){
			i++;
		}
		while((buffer->data[j] > auxKey) && (j >= i)){
			j--;
			fflush(stdout);
		}
		if(i < j){
			fflush(stdout);
			trocar(&buffer->data[i], &buffer->data[j]);
			i++;
			j--;
		}
	}
	trocar(&buffer->data[i], &buffer->data[end]);
		
	quickSort(buffer, ini, i-1, esc);
	quickSort(buffer, i+1, end, esc);
}

void descer(numArray* buffer, int n, int i){
	int maior = i, left, right;
	left = 2*i+1;
	right = left+1;

	if(left<n && buffer->data[left] > buffer->data[maior])
		maior = left;
	if(right<n && buffer->data[right] > buffer->data[maior])
		maior = right;
	if(maior!=i){
		trocar(&buffer->data[i], &buffer->data[maior]);
		descer(buffer, n, maior);
	}
}

void arranjar(numArray* buffer, int n){
	for(int i = n/2-1; i>=0; --i){
		descer(buffer, n, i);
	}		
}

void heapSort(numArray* buffer){
	int n = buffer->size;
	arranjar(buffer, n);
	while(n-1>0){
		trocar(&buffer->data[0], &buffer->data[n-1]);
		n--;
		descer(buffer, n, 0);
	}


}


short int openFile(const char* fileName, FILE** binfile, const char* mode){
	*binfile = fopen(fileName, mode);
	if(*binfile){
		return 1;
	}
	return 0;
}


void insertData(const char* fileName, numArray* buffer){
	struct stat fileStat;
	FILE* binfile;
	int file_dcp;

	openFile(fileName, &binfile, "rb");
	
	if(binfile){
		file_dcp = fileno(binfile);

		fstat(file_dcp, &fileStat);
		buffer->data = malloc(fileStat.st_size);
		buffer->size = fileStat.st_size/sizeof(int32_t);

		if(buffer->data==NULL){
			fprintf(stderr, "Erro ao alocar Memoria\n");
			exit(1);
		}

		fread(buffer->data, sizeof(int32_t), fileStat.st_size/sizeof(int32_t), binfile);

		fclose(binfile);
	}
	else{
		fprintf(stderr, "Erro ao abrir o arquivo: %s\n", fileName);
		exit(EXIT_FAILURE);
	}
}

void insertDatainFile(const char* fileName, numArray* buffer){
	FILE* binFile;
	openFile(fileName, &binFile, "wb");

	if(binFile){
		fwrite(buffer->data, sizeof(int32_t), buffer->size, binFile);
		fclose(binFile);
	}
	else{
		fprintf(stderr, "Erro ao criar o arquivo de saida\n");
		exit(EXIT_FAILURE);
	}
}





int main(int argc, char* argv[]){
	numArray array;
	double timeAux;
	short unsigned esc;
	clock_t begin, end;
	FILE* testes;
	
	if(argc != 4){
		fprintf(stderr,"Formato: (Escolha de algoritmo) (Nome do arquivo de entrada) (Nome do arquivo de saida)");
		return 0;
	}

	esc = atoi(argv[1]);

	if(esc>8||esc<1){
		printf("Escolha de algoritmo invalida");
		return 0;
	}

	insertData( argv[2], &array);

	srand(time(NULL));
	
	begin = clock();

	if(array.data){
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
			case 4:
				mergeSort(&array, 0, array.size-1);
				break;
			case 5:
				quickSort(&array, 0, array.size-1, 2);
				break;
			case 6:
				quickSort(&array, 0, array.size-1, 1);
				break;
			case 7:
				quickSort(&array, 0, array.size-1, 3);
				break;
			case 8:
				heapSort(&array);
				break;
			default:
				printf("Opcao invalida, vetor nao ordenado\n");
				break;
		}
		end = clock();

		/*for(size_t i=0; i<array.size; i++){
			printf("%d ", array.data[i]);
		}*/	
		timeAux = (double)(end-begin)/CLOCKS_PER_SEC;

		insertDatainFile(argv[3], &array);
		openFile("teste.txt", &testes, "a");
		fprintf(testes, "%lf", timeAux);
		if(argv[2][strlen(argv[2])-1]=='a' || argv[2][strlen(argv[2])-1]=='c')
			fputc(',', testes);
		else
			fputc('\n', testes);
		
	}
		

	printf("\nTempo gasto: %lf\n", timeAux);

	free(array.data);
}

short int openFile(const char* fileName, FILE** binfile, const char* mode) {
    // Tenta abrir o arquivo no modo especificado (leitura/escrita).
    *binfile = fopen(fileName, mode);
    if (*binfile) {
        return 1; // Retorna 1 se o arquivo foi aberto com sucesso.
    }
    return 0; // Retorna 0 se falhar ao abrir o arquivo.
}

void insertData(const char* fileName, numArray* buffer) {
    struct stat fileStat; // Estrutura para armazenar informações sobre o arquivo.
    FILE* binfile; // Ponteiro para o arquivo binário.
    int file_dcp; // Descriptor de arquivo.

    openFile(fileName, &binfile, "rb"); // Abre o arquivo em modo de leitura binária.

    if (binfile) {
        file_dcp = fileno(binfile); // Obtém o descriptor de arquivo do ponteiro.

        fstat(file_dcp, &fileStat); // Obtém informações sobre o arquivo.
        buffer->data = malloc(fileStat.st_size); // Aloca memória para os dados.
        buffer->size = fileStat.st_size / sizeof(int32_t); // Define o tamanho do buffer.

        if (buffer->data == NULL) {
            fprintf(stderr, "Erro ao alocar Memoria\n");
            exit(1); // Termina o programa se a alocação falhar.
        }

        fread(buffer->data, sizeof(int32_t), fileStat.st_size / sizeof(int32_t), binfile); // Lê os dados do arquivo.

        fclose(binfile); // Fecha o arquivo.
    } else {
        fprintf(stderr, "Erro ao abrir o arquivo: %s\n", fileName);
        exit(EXIT_FAILURE); // Termina o programa se o arquivo não puder ser aberto.
    }
}

void insertDatainFile(const char* fileName, numArray* buffer) {
    FILE* binFile; // Ponteiro para o arquivo binário.
    openFile(fileName, &binFile, "wb"); // Abre o arquivo em modo de escrita binária.

    if (binFile) {
        fwrite(buffer->data, sizeof(int32_t), buffer->size, binFile); // Escreve os dados no arquivo.
        fclose(binFile); // Fecha o arquivo.
    } else {
        fprintf(stderr, "Erro ao criar o arquivo de saida\n");
        exit(EXIT_FAILURE); // Termina o programa se o arquivo não puder ser criado.
    }
}

int main(int argc, char* argv[]) {
    numArray array; // Declara um array de números.
    double timeAux; // Variável para armazenar o tempo de execução.
    short unsigned esc; // Variável para armazenar a escolha do algoritmo.
    clock_t begin, end; // Variáveis para medir o tempo de execução.
    FILE* testes; // Ponteiro para o arquivo de testes.

    // Verifica se o número de argumentos passados é correto.
    if (argc != 4) {
        fprintf(stderr, "Formato: (Escolha de algoritmo) (Nome do arquivo de entrada) (Nome do arquivo de saida)");
        return 0; // Retorna se o formato estiver incorreto.
    }

    esc = atoi(argv[1]); // Converte a escolha do algoritmo para um inteiro.

    // Verifica se a escolha do algoritmo é válida.
    if (esc > 8 || esc < 1) {
        printf("Escolha de algoritmo invalida");
        return 0; // Retorna se a escolha for inválida.
    }

    insertData(argv[2], &array); // Insere os dados do arquivo de entrada no array.

    srand(time(NULL)); // Inicializa o gerador de números aleatórios.

    begin = clock(); // Inicia a contagem do tempo.

    // Verifica se os dados foram inseridos com sucesso.
    if (array.data) {
        switch (esc) { // Executa o algoritmo de ordenação baseado na escolha do usuário.
            case 1:
                bubbleSort(&array);
                break;

            case 2:
                bubbleSortImprov(&array);
                break;

            case 3:
                insertionSort(&array);
                break;

            case 4:
                mergeSort(&array, 0, array.size - 1);
                break;

            case 5:
                quickSort(&array, 0, array.size - 1, 2);
                break;

            case 6:
                quickSort(&array, 0, array.size - 1, 1);
                break;

            case 7:
                quickSort(&array, 0, array.size - 1, 3);
                break;

            case 8:
                heapSort(&array);
                break;

            default:
                printf("Opcao invalida, vetor nao ordenado\n");
                break;
        }
        end = clock(); // Finaliza a contagem do tempo.

        // Calcula o tempo gasto na execução.
        timeAux = (double)(end - begin) / CLOCKS_PER_SEC;

        insertDatainFile(argv[3], &array); // Insere os dados ordenados no arquivo de saída.
        openFile("teste.txt", &testes, "a"); // Abre (ou cria) o arquivo de teste em modo de adição.
        fprintf(testes, "%lf", timeAux); // Registra o tempo de execução no arquivo.
        if (argv[2][strlen(argv[2]) - 1] == 'a' || argv[2][strlen(argv[2]) - 1] == 'c')
            fputc(',', testes); // Adiciona uma vírgula se o nome do arquivo termina com 'a' ou 'c'.
        else
            fputc('\n', testes); // Adiciona uma nova linha caso contrário.
    }

    printf("\nTempo gasto: %lf\n", timeAux); // Exibe o tempo gasto.

    free(array.data); // Libera a memória alocada para os dados.
}

