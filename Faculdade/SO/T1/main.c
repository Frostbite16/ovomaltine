#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>

#define SHELL_LINE "teste@teste"  // Texto fixo do prompt da shell
#define ARG_MAX 10                // Número máximo de argumentos (não usado explicitamente)

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
            execvp("./treePID", arguments);
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
    char commandLine[256], path[256];

    getcwd(path, 256);  // Obtém diretório inicial

    while (1) {
        // Imprime prompt e lê comando
        printf("[" SHELL_LINE " %s]: ", path);
        fgets(commandLine, 256, stdin);

        commandLine[strlen(commandLine) - 1] = '\0';  // Remove '\n' do final
        commandReader(commandLine, path);              // Processa o comando
    }
    return 0;
}
