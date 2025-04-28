#include<stdio.h>   // I/O padrão
#include<stdlib.h>  // Alocação de memória e utilitários
#include<string.h>  // Manipulação de strings
#include<ctype.h>   // Funções de caractere (isdigit)
#include<dirent.h>  // Leitura de diretórios

#define PROC_DIR "/proc"  // Diretório de informações dos processos

// Nó da árvore de processos
struct processNode {
    size_t processID;         // ID do processo
    char* processName;        // Nome do processo
    char processState;        // Estado (letra)
    size_t parentProcessID;   // ID do processo pai
	size_t numChild;          // Número de filhos
	struct processNode** children;  // Lista de ponteiros para filhos
};

// Cria e inicializa um novo nó de processo a partir do arquivo stat
void newProcessNode(struct processNode** newNode, const char* path, int flag){
	FILE* statfile;
	struct processNode* node = *newNode;
	char buffer[20];

	// Abre arquivo /proc/[pid]/stat
	statfile = fopen(path, "r");
	if(!statfile){
		fprintf(stderr, "Erro ao abrir o arquivo\n");
		exit(EXIT_FAILURE);
	}

	// Reserva espaço para o nome
	node->processName = (char*) malloc(sizeof(char) * 256);
	// Lê pid, nome, estado e ppid
	fscanf(statfile, "%lu %s %c %lu", &node->processID,
			node->processName, &node->processState,
		   &node->parentProcessID);
	// Se flag ativo, anexa o pid ao nome
	if(flag==1) {
		sprintf(buffer, "(%lu)", node->processID);
		strcat(node->processName, buffer);
	}

	// Inicializa filhos
	node->children = NULL;
	node->numChild = 0;
	// Fecha arquivo
	fclose(statfile);
}

// Busca recursiva de um processo na árvore pelo ID
struct processNode* searchProcessInTree(struct processNode** head, size_t processID){
	struct processNode *curProcessNode = *head, *result = NULL;
	// Árvore vazia
	if(!head && !*head){
		return result;
	}
	// Se encontrar, retorna
	if(curProcessNode->processID == processID){
		return curProcessNode;
	}
	// Pesquisa em cada filho
	for(size_t i=0; result==NULL && i<curProcessNode->numChild; i++){
		result = searchProcessInTree(&curProcessNode->children[i], processID);
	}
	return result;
}

// Insere um processo na árvore, ligando ao seu pai
void insertProcessTree(struct processNode** head, const char* path, int flag){
	struct processNode *newNode, *parentNode;

	newNode = malloc(sizeof(struct processNode));
	newProcessNode(&newNode, path, flag);  // Cria nó
	// Se raiz ainda não existe
	if(!*head){
		*head = newNode;
		return;
	}
	// Encontra o pai e anexa como filho
	if((parentNode = searchProcessInTree(head, newNode->parentProcessID))){
		parentNode->numChild++;
		parentNode->children = (struct processNode**)realloc(
			parentNode->children,
			sizeof(struct processNode*) * parentNode->numChild);
		parentNode->children[parentNode->numChild-1] = newNode;
	}
}

// Libera toda a árvore de processos
void freeTree(struct processNode** head){
	struct processNode* node = *head;
	if(node) {
		// Libera filhos primeiro
		for(size_t i=0; i<node->numChild; i++){
			freeTree(&node->children[i]);
		}
		free(node->children);      // Libera lista de filhos
		free(node->processName);   // Libera nome
		free(node);                // Libera nó
	}
}

// Imprime árvore no formato ASCII
void printTree(struct processNode* tree, char* buffer){
	char buffer2[256] = "", newBuf[256] = "";
	struct processNode* curNode = tree;

	// Imprime nome do processo
	printf("%s", curNode->processName);
	// Para cada filho, desenha ramos
	for(size_t i=0; i<curNode->numChild; i++){
		// Prepara indentação
		strcpy(newBuf, buffer);
		for(size_t j=0; j<=strlen(curNode->processName); j++)
			strcat(newBuf, " ");
		strcpy(buffer2, newBuf);
		// Escolhe símbolo vertical ou espaço
		if(curNode->numChild>1 && i<curNode->numChild-1)
			strcat(buffer2, "│ ");
		else
			strcat(buffer2, "  ");

		// Conexões de árvore
		if(curNode->numChild>1){
			if(i==0) {
				printf("─┬");
			} else {
				printf("%s", newBuf);
				if(i==curNode->numChild-1)
					printf("└");
				else
					printf("├");
			}
		} else {
			printf("──");
		}
		printf("─");
		// Chamada recursiva
		printTree(curNode->children[i], buffer2);
	}
	// Quebra linha se folha
	if(!curNode->numChild)
		printf("\n");
}

// Verifica se string é numérica (nome de PID)
int isNumber(const char *name){
	while(*name){
		if(!isdigit(*name))
			return 0;
		name++;
	}
	return 1;
}

int main(int argc, char const *argv[]){
	DIR* procDir = opendir(PROC_DIR);  // Abre diretório /proc
	struct processNode* treeHead = NULL;
	struct dirent* processFile;
	char processPath[256] = "";
	char buffer[256] = "";
	int flag = 0;

	// Valida argumentos
	if (argc>2) {
		printf("treePID: Numero de argumentos inválido\n");
		exit(EXIT_FAILURE);
	}

	// Lê flag se presente
	while((processFile = readdir(procDir)) != NULL){
		if(isNumber(processFile->d_name)){
			strcpy(processPath, PROC_DIR"/");
			strcat(processPath, processFile->d_name);
			strcat(processPath, "/stat");
			if (argc!=1) {
				flag = atoi(argv[1]);
				if (flag==1 || flag==0)
					insertProcessTree(&treeHead, processPath, flag);
				else
					printf("treePID: Argumento Invalido\n");

			}
			else
				insertProcessTree(&treeHead, processPath, 0);

		}
	}
	// Imprime e libera
	printTree(treeHead, buffer);
	freeTree(&treeHead);
	closedir(procDir);

	return 0;
}
