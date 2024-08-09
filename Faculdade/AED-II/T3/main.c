#include <stdio.h> /* printf, scanf */
#include <stdlib.h> /* malloc, free */
#include <unistd.h>
#include "gfx/gfx.h" /* biblioteca de visualização gráfica */
#define SCREEN_SIZEX 1600
#define SCREEN_SIZEY 800
#define NODE_SIZEX 17
#define NODE_SIZEY 14
#define NODE_INTERVAL_SIZE 50
#define OFFSET 5


//estrutura da árvore de busca para a inserção no arquivo
typedef struct SearchTree{
    int data;
    struct SearchTree *leftChild, *rightChild;
}searchTreeNode;
//estrutura da árvore de busca para a inserção no arquivo
typedef struct s_arq_no{
    __int32_t chave:30;
    __uint32_t esq:1;
    __uint32_t dir:1;

}sTreeNode_arq;

// Insere um nó em uma arvóre binária
// caso a chave ja exista no nó a verificação é feita fora da função
int insertNode(searchTreeNode** nodeAdress, int x){ // recebe como parametro um ponteiro pro ponteiro da estrutura, e um valor a ser inserido na árvore
    searchTreeNode *newChild = malloc(sizeof(searchTreeNode));// *currentChild;
    
	newChild->data = x;			 //	Cria um novo nó para ser inserido na árvore
    newChild->leftChild = NULL;  
    newChild->rightChild = NULL; 

    if(*(nodeAdress)==NULL){ // verifica se árvore existe
        *(nodeAdress) = newChild; // atribui o novo nó ao ponteiro da árvore e termina a funçãp
        return 1;
    }
    while((*(nodeAdress)!=NULL)){ 
        //currentChild =*(nodeAdress);
        if(x<(*(nodeAdress))->data){ 					// itera entre os nós da árvore
            nodeAdress=&(*(nodeAdress))->leftChild;		// sempre seguindo as regras da árvore de busca;
        }
        else{
            nodeAdress=&(*(nodeAdress))->rightChild;
        }
    }
    *nodeAdress = newChild; // o nó e inserido em seu devido lugar;
    return 1;
}

// retorna o nó com maior valor da árvore///
searchTreeNode* searchMax(searchTreeNode* currentNode){ // recebe como parametro o ponteiro para a estrutura
    if(currentNode!=NULL){ //itera até o nó mais a direita
        while(currentNode->rightChild!=NULL){
            currentNode = currentNode->rightChild;
        }
    }
    return currentNode;
}

// retorna o nó com menor valor da árvore
searchTreeNode* searchMin(searchTreeNode* currentNode){ // receve como parametro o ponteira para a estrutura
    if(currentNode!=NULL){// itera até o nó mais a esquerda
        while(currentNode->leftChild!=NULL){
            currentNode = currentNode->leftChild;
        }
    }
    return currentNode;
}

//procura um nó de valor x para a estrutura, retorna nulo caso não exista, ou o ponteiro duplo pra esse nó caso exita
searchTreeNode** searchNode(searchTreeNode** root, int x){ // recebe como parametro o ponteiro para a estrutura da arvore e o valor a ser procurado
    searchTreeNode *currentNode, **nodeAdress=root;
	nodeAdress = root, currentNode = *root;	
    if(root!=NULL){
        while((*(nodeAdress)!=NULL)&&((*(nodeAdress))->data!=x)){ // itera até o nó necessario seguindo as regras da arvore de busca
            if(x<currentNode->data){
                nodeAdress = &currentNode->leftChild;
                currentNode = currentNode->leftChild;
            }
            else{
                nodeAdress = &currentNode->rightChild;
                currentNode = currentNode->rightChild;
            }
        }
    }
    return nodeAdress; // retorna o ponteiro que aponta para o nó buscado ou um ponteiro que aponta para NULL
}


// Procura um nó sucessor ao nó de valor x, retorna o endereço desse nó
searchTreeNode** searchSucessor(searchTreeNode* root, int x ){// recebe o ponteiro para a estrutura da árvore e o valor x
    searchTreeNode *currentNode = root, **sucessor = NULL;
    (root->data>x) ? (sucessor=&root) : (sucessor=NULL); // faz uma verificação inicial caso o começo da arvore seja maior que x
    if(currentNode!=NULL){ 
        while((currentNode!=NULL)&&(currentNode->data!=x)){ // itera até achar o nó de valor x e atribui o nó a direita como sucessor
            if(currentNode->data>x){
                if(currentNode->leftChild->data!=x){
                    sucessor = &(currentNode->leftChild);
                }
                currentNode=currentNode->leftChild;
            }
            else{
                currentNode=currentNode->rightChild;
            }
        }   
		if((currentNode!=NULL)&&(currentNode->rightChild!=NULL)){ // verifica se o nó a direita tem descendentes a esquerda e atribui esse nó como sucessor
			sucessor = &currentNode->rightChild;
            currentNode=currentNode->rightChild;
            while(currentNode->leftChild!=NULL){
                sucessor = &currentNode->leftChild;
                currentNode=currentNode->leftChild;
            }
            return sucessor;
		}
		else if(currentNode==NULL) // caso o nó de valor x não exista
			return NULL;
    }
    return sucessor;
}

searchTreeNode** searchAntecessor(searchTreeNode* root, int x ){ // simétrico ao sucessor
    searchTreeNode *currentNode = root, **antecessor = NULL;
    (root->data<x) ? (antecessor=&root) : (antecessor=NULL);
    if(currentNode!=NULL){
        
        while(currentNode->data!=x){
            if(currentNode->data<x){
                if(currentNode->rightChild->data!=x){
                    antecessor = &(currentNode->rightChild);
                }
                currentNode=currentNode->rightChild;
            }
            else{
                currentNode=currentNode->leftChild;
            }
        }

		if((currentNode!=NULL)&&(currentNode->leftChild!=NULL)){
            antecessor = &currentNode->leftChild;
            currentNode=currentNode->leftChild;
            while(currentNode->rightChild!=NULL){
                antecessor = &currentNode->rightChild;
                currentNode=currentNode->rightChild;
            }
            return antecessor;
        }
		else if(currentNode==NULL)
			return NULL;

    }
    return antecessor;
}

// remove o nó de valor x
int removeNode(searchTreeNode** nodeAdress, int x){ // recebe o endereço do ponteiro da estrutura e um valor x
    searchTreeNode *currentNode, *root, *sucessor, **adressToSucessor;
	root = *(nodeAdress);
    nodeAdress = searchNode(nodeAdress, x); // procura o nó de valor x na arvore e retorna o endereço que aponta pra esse nó
    if(*(nodeAdress)!=NULL){ 
            currentNode = *(nodeAdress);
            // verifica se o nó a ser removido tem os dois filhos //
			if(currentNode->leftChild==NULL){ 
				*(nodeAdress) = currentNode->rightChild;
				free(currentNode);
                return 1;
            }
            if(currentNode->rightChild==NULL){
                *(nodeAdress) = currentNode->leftChild;
                free(currentNode);
                return 1;
            }
			
            adressToSucessor = searchSucessor(root, x); // procura o sucessor desse nó
			sucessor = *(adressToSucessor);           
			if(currentNode->rightChild==sucessor){ // verifica se o sucessor é o nó imediatamente a direita	
				currentNode->rightChild = sucessor->rightChild;
				currentNode->data=sucessor->data;
				free(sucessor);
				//*(nodeAdress)=sucessor;
				return 1;
			}
			*(adressToSucessor) = sucessor->rightChild; // caso o sucessor do nó a ser removido for filho de seu filho
			currentNode->data=sucessor->data;
			free(sucessor);
			return 1;
    }
    return 0;
}

// desaloca os nós da arvóre de forma recursiva
void freeTree(searchTreeNode **adressToNode){
    searchTreeNode *currentNode = *(adressToNode);
    if(currentNode->leftChild!=NULL){
        freeTree(&currentNode->leftChild);
    }
    if(currentNode->rightChild!=NULL){
        freeTree(&currentNode->rightChild);
    }
	free(currentNode);
	*(adressToNode) = NULL;
}


// cria um no na estrutura certa para ser inserida no arquivo
void createFileNode(sTreeNode_arq *treeStruct, searchTreeNode* currentNode){
   
		treeStruct->chave = currentNode->data;
		(currentNode->leftChild!=NULL) ? (treeStruct->esq=1):(treeStruct->esq=0);
	   	(currentNode->rightChild!=NULL) ? (treeStruct->dir=1):(treeStruct->dir=0);	

}

FILE* openBinFile(char* fileName, char* mode){ // verifica se o arquivo existe, caso exista abre o arquivo
	if(access(fileName, F_OK)==0){
		return fopen(fileName, mode);
	}
	return NULL;
} 

void writeBinFile(FILE* binFile, searchTreeNode* currentNode){ //insere os nós no arquivo
    sTreeNode_arq treeStruct;
    if(currentNode!=NULL){
        createFileNode(&treeStruct, currentNode);

        fwrite(&treeStruct,sizeof(sTreeNode_arq),1,binFile);
		if(currentNode->leftChild!=NULL){
			writeBinFile(binFile, currentNode->leftChild);
        }
        if(currentNode->rightChild!=NULL){
            writeBinFile(binFile, currentNode->rightChild);
        }
    }
}

void readBinFile(FILE* binFile, searchTreeNode** currentNode){ //cria uma estrutura de arvore lendo as estruturas dentro do arquivo
	sTreeNode_arq treeStruct;
	fread(&treeStruct, sizeof(sTreeNode_arq), 1, binFile);
	if(*(searchNode(currentNode, treeStruct.chave)) == NULL){
		insertNode(currentNode, treeStruct.chave);
	}	
	if(treeStruct.esq==1){	
		readBinFile(binFile, currentNode);	
	}
	//fseek(binFile, sizeof(sTreeNode_arq), SEEK_CUR);
	if(treeStruct.dir==1){
		readBinFile(binFile, currentNode);		
	}
	
}




void drawNode(unsigned x, unsigned y, int chave){ // desenha um na tela
    char txt[12];
	int width, height; 
    sprintf(txt, "%d", chave); 
		
	gfx_set_color(255,255,0);
    gfx_filled_rectangle(x-NODE_SIZEX, y-NODE_SIZEY, x+NODE_SIZEX, y+NODE_SIZEY);
	gfx_set_color(255,0,0);

	gfx_rectangle(x-NODE_SIZEX, y-NODE_SIZEY, x+NODE_SIZEX, y+NODE_SIZEY);
    gfx_set_color(80,3,128);
	
	gfx_set_font_size(15);
	gfx_get_text_size(txt, &width, &height); //obtem a altura e largura da fonte para centralizar o texto/
	
    gfx_text(x-width/2,y-height/2,txt);

		

}

void gfxCreateTree(searchTreeNode* currentNode, unsigned rate, unsigned X, unsigned Y, int fatherX){ // funcao que determina aonde devem ser inseridos os nós da tela
	
	
	if(currentNode!=NULL){   		
		gfxCreateTree(currentNode->leftChild, rate/2 , X  - rate, Y + NODE_INTERVAL_SIZE, X);
        gfxCreateTree(currentNode->rightChild, rate/2, X  + rate, Y + NODE_INTERVAL_SIZE, X);
  		
	  	if(fatherX>=0){
			gfx_set_color(255, 255, 255);
			gfx_line(fatherX, Y - NODE_INTERVAL_SIZE, X, Y);
		}
	
	    drawNode(X,Y,currentNode->data);
	}
}
int main(){	
    // variaveis
	searchTreeNode *root = NULL;
	unsigned short esc=0;
	int num;
	searchTreeNode **sucessor, **antecessor;
	FILE* binfile;
	
	gfx_init(SCREEN_SIZEX, SCREEN_SIZEY, "Trabalho 3"); // cria a tela
	do{
		
		
		switch(esc){
			// menu	
			case 1:				
				while(esc!=0){
					printf("[1] Inserção.\n");
					printf("[2] Remocao.\n");
					printf("[3] Busca pelo sucessor.\n");
					printf("[4] Busca pelo antecessor.\n");
					printf("[5] Busca pelo maior no da arvore.\n");
					printf("[6] Busca pelo menor no da arvore.\n");
					printf("[7] Desalocar arvore\n");

					printf("[0] Voltar.\n");
					printf("Escolha uma das opcoes: \n");
					scanf("%hu", &esc);
					if((esc!=0)&&(esc!=1)&&(root==NULL)){ // verifica se a arvore existe
						esc=1;
						printf("Eh necessario um no na arvore antes de realizar as demais operacoes\n");
					}

					switch(esc){

						case 0:
							break;
						case 1:
							printf("Qual numero deseja inserir?: \n");
							scanf("%d", &num);
							if(*(searchNode(&root, num))==NULL){ // verifica se o numero a ser inserido existe na arvore
								insertNode(&root, num);
								gfx_clear();
								gfxCreateTree(root, SCREEN_SIZEX/4,SCREEN_SIZEX/2, NODE_INTERVAL_SIZE, -1);
								gfx_paint();
								printf("Numero Inserido\n");
							}
							else
								printf("Operacao nao permitida: Numero encontrado na arvore\n");
							break;
						case 2:			
							printf("Qual numero deseja remover?: \n");
							scanf("%d", &num);
							if(removeNode(&root, num)==1){ 
								printf("Operacao de remocao executada com exito\n");
								gfx_clear();
								gfxCreateTree(root, SCREEN_SIZEX/4,SCREEN_SIZEX/2, NODE_INTERVAL_SIZE, -1);
								gfx_paint();
							}
							else{
								printf("Operacao invalida\n");
							}
							break;	
						case 3:
							printf("Qual numero deseja saber o sucessor?: \n");
							scanf("%d", &num);
							sucessor = searchSucessor(root, num);
							if(sucessor!=NULL)
								printf("O sucessor do numero %d eh o %d\n", num, (*sucessor)->data);	
							else
								printf("Sucessor nao existente\n");
							break;
						case 4:
							printf("Qual numero deseja saber o antecessor?: \n");
							scanf("%d", &num);
							antecessor = searchAntecessor(root,num);
							if(antecessor!=NULL)
								printf("O Antecessor do numero %d eh o %d\n", num, (*antecessor)->data);				
							else
								printf("Antecessor nao existente\n");
							break;
						case 5:
							printf("O maior elemento da arvore eh o numero %d\n", searchMax(root)->data);
							break;
						case 6:
							printf("O menor elemento da arvore eh o numero %d\n", searchMin(root)->data);
							break;
						case 7:
							freeTree(&root);
							gfx_clear();
							gfx_paint();
							break;
						default:
							printf("Opcao invalida\n");
							break;
					}
				}
			break;	
			case 2:
				while(esc!=0){

					printf("[1] Gravar arvore em arquivo binario\n");
					printf("[2] Leitura de arquivo binario\n");
					printf("[0] Voltar\n");
					printf("Escolha uma das opcoes:");
					scanf("%hu", &esc);
								
					switch(esc){
						case 1:
							if(root!=NULL){
								binfile = fopen("binfile.bin", "wb");
								writeBinFile(binfile, root);
								fclose(binfile);
							}
							else{
								printf("Arvore nao encontrada!!");
							}
							break;
						case 2:
							binfile = fopen("binfile.bin", "rb");
							if(binfile!=NULL){
								if(root!=NULL){
									printf("Arvore existente, deseja limpar a arvore para a leitura de arquivo? [1] sim, [2] nao: ");
									scanf("%hu",&esc);
									while(esc!=2){
										switch(esc){
											case 1:
												freeTree(&root);
												esc=2;
												break;
											case 2:
												break;
											default:
												printf("Escolha invalida\n");
										}
									}
								}
								readBinFile(binfile, &root);
								gfx_clear();
								gfxCreateTree(root, SCREEN_SIZEX/4,SCREEN_SIZEX/2, NODE_INTERVAL_SIZE, -1);
								gfx_paint();
								fclose(binfile);

							}
							else
								printf("arquivo nao existente!!");
							break;
						case 0:
							break;
						default:
							printf("Opcao invalida\n");
							break;
					}
				}
		
		}
	
		printf("[1] Operacoes na arvore.\n");
		printf("[2] Leitura ou gravacao de arquivo.\n");
		printf("[0] Sair.\n");
		printf("Escolha uma das opcoes: \n");			
		scanf("%hu", &esc);
	
	}while(esc!=0);
	
	if(root!=NULL){ // remove a árvore caso não for removida na duração do programa
		freeTree(&root);
	}
}
