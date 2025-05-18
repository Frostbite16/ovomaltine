#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include<ctype.h>   // Funções de caractere (isdigit)
#include<dirent.h>  // Leitura de diretórios
#include <pwd.h>

#define PROC_DIR "/proc"  // Diretório de informações dos processos
#define ARG_MAX 10                // Número máximo de argumentos (não usado explicitamente)

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

int runTreePID(char* argument){
    DIR* procDir = opendir(PROC_DIR);  // Abre diretório /proc
    struct processNode* treeHead = NULL;
    struct dirent* processFile;
    char processPath[256] = "";
    char buffer[256] = "";
    int flag;
    // Lê flag se presente
    if (argument == NULL)
        flag = 0;
    else
        flag = atoi(argument);
    if(flag!=1 && flag!=0) {
        printf("treePID: Argumento Invalido\n");
        return 1;
    }

    while((processFile = readdir(procDir)) != NULL){
        if(isNumber(processFile->d_name)){
            strcpy(processPath, PROC_DIR"/");
            strcat(processPath, processFile->d_name);
            strcat(processPath, "/stat");
            insertProcessTree(&treeHead, processPath, flag);

        }
    }
    // Imprime e libera
    printTree(treeHead, buffer);
    freeTree(&treeHead);
    closedir(procDir);

    return 1;
}

// Função que processa e executa um comando recebido
int commandReader(char* command, char* curPath) {
    char *tokens, *primeCommand, **arguments, **temp;
    short unsigned clock = 1, flag = 0;  // clock: contador de argumentos, flag: execução em background
    pid_t pid;

    // Verifica se o comando termina com '&' para executar em background
    if (command[strlen(command) - 1] == '&') {
        command[strlen(command) - 1] = '\0';  // Remove o '&' do comando
        flag = 1;                              // Marca execução em background
    }

    tokens = strtok(command, " ");  // Separa o comando pelo primeiro espaço
    if (tokens == NULL)
        return 0;  // Se nenhum comando, retorna

    // Comando especial: exit fecha o terminal
    if (!strcmp(tokens, "exit")) {
        printf("Fechando Terminal\n");
        exit(EXIT_SUCCESS);
    }

    primeCommand = tokens;  // Armazena o nome do comando principal

    // Inicializa lista de argumentos com espaço para o comando em si
    arguments = malloc(sizeof(char*));
    arguments[0] = malloc(sizeof(char) * strlen(tokens) + 1);

    tokens = strtok(NULL, " ");  // Próximo token

    // Loop para coletar todos os argumentos adicionais
    while (tokens != NULL) {
        // Realoca espaço para mais um argumento
        temp = realloc(arguments, sizeof(char*) * (clock + 1));
        if (temp) {
            arguments = temp;
        } else {
            // Falha na realocação: libera memória e sai com erro
            for (int i = clock - 1; i >= 0; i--) {
                free(arguments[i]);
            }
            free(arguments);
            fprintf(stderr, "Erro alocando memória.\n");
            exit(EXIT_FAILURE);
        }

        // Aloca e copia o argumento atual
        arguments[clock] = malloc(sizeof(char) * strlen(tokens) + 1);
        strncpy(arguments[clock], tokens, strlen(tokens) + 1);
        tokens = strtok(NULL, " ");  // Próximo token
        clock++;
    }

    // Adiciona espaço para o ponteiro NULL ao final dos argumentos
    if (!((temp = realloc(arguments, sizeof(char*) * (clock + 1))))) {
        for (int i = clock - 1; i >= 0; i--) {
            free(arguments[i]);
        }
        free(arguments);
        fprintf(stderr, "Erro alocando memória.\n");
        exit(EXIT_FAILURE);
    }
    arguments = temp;

    arguments[clock] = NULL;               // Finaliza lista com NULL
    strcpy(arguments[0], primeCommand);    // Garante que o primeiro é o nome do comando

    // Cria novo processo para executar o comando
    pid = fork();
    if (pid == 0) {  // Código do filho
        short value_check = 0;
        // Tratamento especial para comando treePID
        if (!strcmp(primeCommand, "treePID")) {
            if (clock > 2) {
                printf("treePID: Numero de argumentos invalido\n");
                exit(1);
            }
            //execvp("./treePID", arguments);
            if (clock==2) {
                runTreePID(arguments[1]);
            }
            else {
                runTreePID(NULL);
            }

            exit(EXIT_SUCCESS);
        }
        // Tratamento especial para comando cd
        if (!strcmp(primeCommand, "cd")) {
            if (clock > 2 || clock < 1) {
                printf("cd: Quantidade de argumentos invalidos\n");
                exit(EXIT_FAILURE);
            }
            chdir(arguments[1]);          // Muda diretório atual
            getcwd(curPath, 256);         // Atualiza variável de caminho
            exit(EXIT_SUCCESS);
        }
        // Execução padrão de outros comandos
        value_check = execvp(primeCommand, arguments);
        if (value_check == -1) {
            printf("Erro na execucao do programa\n");
        }
    }

    // Se não for background, espera processo filho terminar
    if (flag == 0) {
        waitpid(pid, NULL, 0);
    }

    // Libera memória alocada para argumentos
    for (int i = 0; i <= clock; i++) {
        free(arguments[i]);
    }
    free(arguments);

    return 0;
}

int main() {
    char commandLine[256], path[256], login[256], hostname[25];
    strcpy(login, getlogin());
    gethostname(hostname, sizeof(hostname));

    getcwd(path, 256);  // Obtém diretório inicial

    while (1) {
        // Imprime prompt e lê comando
        printf("[%s@%s %s]: ", login,hostname,path);
        fgets(commandLine, 256, stdin);

        commandLine[strlen(commandLine) - 1] = '\0';  // Remove '\n' do final
        commandReader(commandLine, path);              // Processa o comando
    }
    return 0;
}
