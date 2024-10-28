#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <stdbool.h>

#define MAX_PALAVRA 20
#define NO_TRANSICAO -1


typedef struct array{
	char** data;
	size_t size;
}array;

typedef struct transition{
	int* states;
	size_t size;
} transition;

typedef struct AF{
	array estados, alfabeto, estadosFinais;
	transition **transitionTable;
	char estadoInicial[MAX_PALAVRA];
	short unsigned epsilon;
}automata;

void initialize(array* elemArray){
	elemArray->size = 0;
	elemArray->data = NULL;
}

void initializeTransition(transition* trans){
	trans->size = 0;
	trans->states = NULL;
}

void novoElem(array* elemArray, const char* palavra){
	char** array = elemArray->data;
	if(elemArray->size==0){
		elemArray->data = malloc(sizeof(char *)*(elemArray->size+1));
		elemArray->data[0] = (char*)(malloc(strlen(palavra)+1)); 
		strcpy(elemArray->data[0], palavra);
		elemArray->size++;
	}
	else{
		elemArray->data = realloc(elemArray->data, sizeof(char *)*(elemArray->size+1));
		elemArray->data[elemArray->size] = (char *)(malloc(strlen(palavra)+1));
		strcpy(elemArray->data[elemArray->size], palavra);
		elemArray->size++;
	}
}

void freeArray(array* elemArray){
	if((elemArray!=NULL)&&(elemArray->data!=NULL)){
		for(int i=0; i<elemArray->size; i++){
			if(elemArray->data[i] != NULL)
				free(elemArray->data[i]);
		}
		free(elemArray->data);
	}
}

void freeTransitionTable(transition** table, size_t numStates, size_t numSymbols){
	for(size_t i=0; i<numStates; i++){
		for(size_t j=0; j<numSymbols; j++){
			free(table[i][j].states);
		}
		free(table[i]);
	}
	free(table);
}

void processArray(char word[MAX_PALAVRA]){ // Remove os espaços da string
	char buffer[MAX_PALAVRA], *token;
	strcpy(buffer, word);

	token = strtok(buffer, " ");
	strcpy(word, token);
	while(token!=NULL){
		token = strtok(NULL, " ");
		if(token!=NULL)
			strcat(word, token);
	}	
	
}

void wordToArray(char word[MAX_PALAVRA], array *bDados){
	char* token = strtok(word, ",");
	while(token!=NULL){
		novoElem(bDados, token);
		token = strtok(NULL, ",");
	}
}

int searchArray(array *bDados, const char entrada[MAX_PALAVRA]);

void wordToArraySearched(char word[MAX_PALAVRA], array* src, array* dest){
	char* token = strtok(word, ",");
	while(token!=NULL){
		if(searchArray(dest, token)!=-1)
			novoElem(src, token);
		token = strtok(NULL, ",");
	}
}

int searchArray(array *bDados, const char entrada[MAX_PALAVRA]){
	size_t j=0, i=0;
	while(bDados->size>i){
		if(strcmp(bDados->data[i], entrada)==0)
			return i;
		i++;
	}	
	return -1;
}

void addTransition(transition* trans, int stateIndex){
	if(trans->size==0){
		trans->states = malloc(sizeof(int));
	}
	else{
		trans->states = realloc(trans->states, (trans->size + 1) * sizeof(int));
	}
	trans->states[trans->size] = stateIndex;
	trans->size++;
}

void createTransitionTable(transition** transitionTable, array* estados, array* alfabeto){
	char entrada[MAX_PALAVRA], *token;
	int estadoAtual;
	short unsigned check=0;
	for(size_t i = 0 ;i<estados->size; i++){
		for(size_t j = 0; j<alfabeto->size; j++){
			while(getchar()!='\n');

			printf("Transição do estado %s com o simbolo %s\n", estados->data[i], alfabeto->data[j]);
			printf("OBS: Separe mais de um estado por virgula, ou um estado fora do alfabeto para transição Vazia\n");
			scanf("%s", entrada);
			processArray(entrada);

			token = strtok(entrada, ",");
			check=0;
			while(token != NULL){
				estadoAtual = searchArray(estados, token);
				if(estadoAtual != -1){
					addTransition(&transitionTable[i][j], estadoAtual); 
					check=1;
				}
				else{
					printf("Estado %s invalido, ignorando\n", token);
				}
				token = strtok(NULL, ",");
			}
			if(check==0){
				addTransition(&transitionTable[i][j], NO_TRANSICAO);
			}
		}
	}
}

void initializeAF(automata* newAutomata){
	initialize(&newAutomata->alfabeto);
	initialize(&newAutomata->estadosFinais);
	initialize(&newAutomata->estados);
	newAutomata->epsilon = 0;
}

void freeAF(automata* newAutomata){
	freeTransitionTable(newAutomata->transitionTable, newAutomata->estados.size, newAutomata->alfabeto.size);
	freeArray(&newAutomata->alfabeto);
	freeArray(&newAutomata->estadosFinais);
	freeArray(&newAutomata->estados);
}

void cleanAutomata(automata* newAutomata){
	freeAF(newAutomata);
	initializeAF(newAutomata);
}

void displayArray(array *element){
	printf("{");
	for(size_t i = 0; i<element->size; i++){
		if(i<element->size-1)
			printf("%s,",element->data[i]);
		else
			printf("%s}\n", element->data[i]);
	}
}

void displayAutomata(automata newAutomata){
	array *estado = &newAutomata.estados, *alfabeto = &newAutomata.alfabeto, *estadosFinais = &newAutomata.estadosFinais;
	
	printf("Estados: ");
	displayArray(estado);
	
	printf("Alfabeto: ");
	displayArray(alfabeto);
	
	printf("Estados Finais: ");
	displayArray(estadosFinais);
	
	printf("Estado inicial: %s\n", newAutomata.estadoInicial);

	for(size_t i = 0; i<estado->size; i++){
		for(size_t j = 0; j<alfabeto->size; j++){
			printf("Transição do estado %s com o simbolo %s: ", estado->data[i], alfabeto->data[j]);
			for(size_t k = 0; k<newAutomata.transitionTable[i][j].size; k++){
				if(newAutomata.transitionTable[i][j].states[k]==NO_TRANSICAO)
					printf("Vazio");
				else
					printf("%s", estado->data[newAutomata.transitionTable[i][j].states[k]]);
			}
			printf("\n");
		}
	}
}

short unsigned isitDFA(automata newAutomata){
	if(!newAutomata.epsilon){
		for(size_t i = 0; i < newAutomata.estados.size; i++){
			for(size_t j = 0; j < newAutomata.alfabeto.size; j++){
				if(newAutomata.transitionTable[i][j].size>1){
					return 0;
				}
			}
		}
		return 1;
	}
	return 0; 
}

// Function to calculate epsilon closure of a set of NFA states
void epsilonClosure(automata* nfa, bool* closure, int state) {
    if (closure[state]) return; // Already included in closure

    closure[state] = true;

    // Check epsilon transitions (assuming epsilon is represented by the last symbol in the alphabet)
    int epsilonIndex = nfa->alfabeto.size - 1; // Last symbol is epsilon if epsilon is present
    if (nfa->epsilon && epsilonIndex >= 0) {
        for (size_t i = 0; i < nfa->transitionTable[state][epsilonIndex].size; i++) {
            int nextState = nfa->transitionTable[state][epsilonIndex].states[i];
            if (nextState != NO_TRANSICAO) {
                epsilonClosure(nfa, closure, nextState);
            }
        }
    }
}

bool areSetsEqual(bool* set1, bool* set2, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (set1[i] != set2[i]) {
            return false;
        }
    }
    return true;
}

void convertToDFA(automata* nfa, automata* dfa) {
    array* nfaStates = &nfa->estados;
    array* nfaAlphabet = &nfa->alfabeto;
    array* nfaFinalStates = &nfa->estadosFinais;

    // DFA initialization
    initialize(&dfa->estados);
    initialize(&dfa->alfabeto);
    initialize(&dfa->estadosFinais);
    dfa->epsilon = 0;

    // Copy alphabet (excluding epsilon, if present)
    for (size_t i = 0; i < (nfa->epsilon ? nfaAlphabet->size - 1 : nfaAlphabet->size); i++) {
        novoElem(&dfa->alfabeto, nfaAlphabet->data[i]);
    }

    // Create the first DFA state as the epsilon closure of the NFA's initial state
    bool** dfaStates = malloc(sizeof(bool*) * 100);  // For simplicity, assume max 100 DFA states
    int dfaStateCount = 0;

    // Initialize epsilon closure for initial state
    bool* initialClosure = calloc(nfaStates->size, sizeof(bool));
    int initialStateIndex = searchArray(nfaStates, nfa->estadoInicial);
    epsilonClosure(nfa, initialClosure, initialStateIndex);

    // Add this closure as the first DFA state
    dfaStates[dfaStateCount++] = initialClosure;

    // DFA creation loop
    for (int i = 0; i < dfaStateCount; i++) {
        bool* currentDFAState = dfaStates[i];

        // Loop through alphabet symbols (excluding epsilon)
        for (size_t j = 0; j < dfa->alfabeto.size; j++) {
            bool* newState = calloc(nfaStates->size, sizeof(bool));

            // Find transitions for all NFA states in the current DFA state
            for (size_t k = 0; k < nfaStates->size; k++) {
                if (currentDFAState[k]) {
                    for (size_t l = 0; l < nfa->transitionTable[k][j].size; l++) {
                        int nfaNextState = nfa->transitionTable[k][j].states[l];
                        if (nfaNextState != NO_TRANSICAO) {
                            epsilonClosure(nfa, newState, nfaNextState);
                        }
                    }
                }
            }

            // Check if the new state already exists in the DFA
            bool stateExists = false;
            for (int s = 0; s < dfaStateCount; s++) {
                if (areSetsEqual(dfaStates[s], newState, nfaStates->size)) {
                    stateExists = true;
                    break;
                }
            }

            // If the state is new, add it to the DFA
            if (!stateExists) {
                dfaStates[dfaStateCount++] = newState;
            } else {
                free(newState);
            }
        }
    }

    // Mark final states in DFA
    for (int i = 0; i < dfaStateCount; i++) {
        bool* dfaState = dfaStates[i];
        for (size_t j = 0; j < nfaFinalStates->size; j++) {
            int nfaFinalStateIndex = searchArray(nfaStates, nfaFinalStates->data[j]);
            if (dfaState[nfaFinalStateIndex]) {
                // This DFA state is final
                char dfaStateName[MAX_PALAVRA];
                sprintf(dfaStateName, "DFA State %d", i);
                novoElem(&dfa->estadosFinais, dfaStateName);
                break;
            }
        }
    }

    // Free dynamically allocated memory for DFA state tracking
    for (int i = 0; i < dfaStateCount; i++) {
        free(dfaStates[i]);
    }
    free(dfaStates);
}

int main(){
	automata newAutomata;

	size_t esc=0, clock, check=0;
	char entrada[MAX_PALAVRA];

	initializeAF(&newAutomata);

	do{
		switch(esc){
			case 0:
				break;
			case 1:
				printf("Inserir estados, Não é permitido o simbolo '-'\n");
				printf("Digite somente um simbolo separado por virgula: ");
				while(getchar()!='\n');
				
				scanf("%s", entrada);
				
				processArray(entrada);
				wordToArray(entrada, &newAutomata.estados);

				printf("Inserir alfabeto\n");
				printf("Digite somente um simbolo separado por virgula: ");
				while(getchar()!='\n');
				
				scanf("%s", entrada);
				
				processArray(entrada);
				wordToArray(entrada, &newAutomata.alfabeto);

				printf("Digite os Estados Finais, separados, por virgula: ");
				while(getchar()!='\n');
				scanf("%s", entrada);

				processArray(entrada);
				wordToArraySearched(entrada, &newAutomata.estadosFinais, &newAutomata.estados);

				printf("Digite o estado inicial: ");
				while(getchar()!='\n');
				while(check!=1){
					scanf("%s", entrada);
					if(searchArray(&newAutomata.estados, entrada)!=-1){
						check=1;
						strcpy(newAutomata.estadoInicial, entrada);
					}
					else{
						printf("Estado não encontrado, Digite novamente: ");
						check=0;
					}
				}
				check=0;

				printf("Deseja adicionar um epsilon ao automato [1] sim, [0] não\n");
				while(getchar()!='\n');
				scanf("%lu", &esc);
				while(esc==1){
					while(getchar()!='\n');
					printf("Insira um simbolo que não está no alfabeto: ");
					scanf("%s",entrada);
					if(searchArray(&newAutomata.alfabeto, entrada)!=-1){
						esc=1;
					}
					else{
						esc=0;
						novoElem(&newAutomata.alfabeto, entrada);
						newAutomata.epsilon = 1;
					}
				}		

				newAutomata.transitionTable = (transition**)malloc(newAutomata.estados.size*sizeof(transition*));
				for(size_t i=0; i<newAutomata.estados.size;i++){
					newAutomata.transitionTable[i] = (transition*)malloc(newAutomata.alfabeto.size * sizeof(transition));
					for(size_t j = 0; j<newAutomata.alfabeto.size; j++){
						initializeTransition(&newAutomata.transitionTable[i][j]);
					}
				}
				createTransitionTable(newAutomata.transitionTable, &newAutomata.estados, &newAutomata.alfabeto);
				break;
			case 2:
				displayAutomata(newAutomata);
				break;
			case 3:
				cleanAutomata(&newAutomata);	
			case 4:
				if(isitDFA(newAutomata)){
					printf("O automato já é um AFD\n");
				}
				else{
					automata dfa;
					convertToDFA(&newAutomata, &dfa);
					displayAutomata(dfa);
					freeAF(&dfa);
				}
				break;

		}
		printf("[1] Criar AFN\n");
		printf("[2] Mostrar Automato\n");
		printf("[3] Limpar automata\n");
		printf("[4] Converter Automata para DFA\n");
		printf("[0] Sair\n");
		scanf("%lu", &esc);
	}while(esc!=0);
}
