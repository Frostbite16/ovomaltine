#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "gfx/gfx.h"
#define SCREEN_SIZEX 1600
#define SCREEN_SIZEY 800
#define NODE_SIZEX 26
#define NODE_SIZEY 23
#define NODE_INTERVAL_SIZE 50
#define OFFSET

typedef struct s_no

{

    int32_t chave:28;

    int32_t bal:2;

    int32_t reservado:2; /* sem uso */

    struct s_no* esq;

    struct s_no* dir;

} AVLtree;

typedef struct s_arq_no
{

    int32_t chave:28;

    int32_t bal:2;

    uint32_t esq:1;
	
    uint32_t dir:1;

}AVLtree_arq;


// declara um novo no e define seus atributos
void newNode(AVLtree** node, int32_t x){
	*node = malloc(sizeof(AVLtree));
	(*node)->dir = NULL;
	(*node)->esq = NULL;
	(*node)->bal = 0;
	(*node)->chave = x;

}

void rotacaoEsqSimp(AVLtree** root){
	AVLtree *ptu, *currentNode;
	int32_t chaveAux;
	currentNode = *root;
	
	chaveAux = currentNode->chave;
	ptu = currentNode->dir;
	currentNode->dir = ptu->dir;
	ptu->dir = ptu->esq;
	ptu->esq = currentNode->esq;
	currentNode->esq = ptu;
	currentNode->chave = ptu->chave;
	ptu->chave = chaveAux;
}

void rotacaoDirSimp(AVLtree** root){
	AVLtree *ptu, *currentNode;
	int32_t chaveAux;
	currentNode = *root;

	chaveAux = currentNode->chave;
	ptu = currentNode->esq;
	currentNode->esq = ptu->esq;
	ptu->esq = ptu->dir;
	ptu->dir = currentNode->dir;
	currentNode->dir = ptu;
	currentNode->chave = ptu->chave;
	ptu->chave = chaveAux;

}

void rotacaoEsq(AVLtree** root, unsigned short *h){
	AVLtree *ptu, *ptv, *currentNode;
	int32_t chaveAux;
	currentNode = *root;
	ptu = currentNode->dir;
	switch(ptu->bal){
		// Rotação simples
		case 1:
			rotacaoEsqSimp(root);
			ptu->bal = 0;
			currentNode->bal = 0;
			*h = 1;
			break;
		case 0:
			rotacaoEsqSimp(root);
			ptu->bal = 1;
			currentNode->bal = -1;
			*h = 0;
			break;
		// Rotação dupla
		case -1:
			ptv = ptu->esq;
			chaveAux = currentNode->chave;
			ptu->esq = ptv->dir;
			ptv->dir = ptv->esq;
			ptv->esq = currentNode->esq;
			currentNode->esq = ptv;
			currentNode->chave = ptv->chave;
			ptv->chave = chaveAux;
			switch(ptv->bal){
				case 1:
					ptv->bal = -1;
					ptu->bal = 0;
					break;
				case 0:
					ptv->bal = 0;
					ptu->bal = 0;
					break;
				case -1:
					ptv->bal = 0;
					ptu->bal = 1;
					break;
			}
			currentNode->bal = 0;
			*h = 1;

	}	
}

void rotacaoDir(AVLtree** root, unsigned short *h){
	AVLtree *ptu, *ptv, *currentNode; 
	int32_t chaveAux;
	currentNode = *root;
	chaveAux = currentNode->chave;
	ptu = currentNode->esq;

	switch(ptu->bal){
		case -1:
			rotacaoDirSimp(root);
			currentNode->bal = 0;
			ptu->bal = 0;
			*h = 1;
			break;
		case 0:
			rotacaoDirSimp(root);
			currentNode->bal = -1;
			ptu->bal = 1;
			*h = 0;
			break;
		case 1:
			ptv = ptu->dir;
			ptu->dir = ptv->esq;
			ptv->esq = ptv->dir;
			ptv->dir = currentNode->dir;
			currentNode->dir = ptv;
			currentNode->chave = ptv->chave;
			ptv->chave = chaveAux;
			switch(ptv->bal){
				case -1:
					ptv->bal = 1;
					ptu->bal = 0;
					break;
				case 0:
					ptv->bal = 0;
					ptu->bal = 0;
					break;
				case 1:
					ptv->bal = 0;
					ptu->bal = -1;
					break;
			}
			currentNode->bal = 0;
			*h = 1;

	}
	
}

// Algoritmo para inserção de um no novo na AVL
void insertAVL(AVLtree **raiz, int32_t x, unsigned short *h){
	AVLtree* node, **currentNode;
	currentNode = raiz;	
	if(*currentNode==NULL){
		newNode(&node, x);
		*currentNode = node;
		*h = 1;
	}
	else if((*currentNode)->chave>x){
		insertAVL((&(*currentNode)->esq), x, h);
		if(*h){
			switch((*currentNode)->bal){
				case 1:
					(*currentNode)->bal = 0;
					*h = 0;
					break;
				case 0:
					(*currentNode)->bal = -1;
					break;
				case -1:
					// Caso 1
					rotacaoDir(currentNode, h);
					*h=0;
					break;
			}
		}
	}
	else if((*currentNode)->chave<x){
		insertAVL(&((*currentNode)->dir), x, h);
		if(*h){
			switch((*currentNode)->bal){
				case -1:
					(*currentNode)->bal = 0;
					*h = 0;
					break;
				case 0:
					(*currentNode)->bal = 1;
					break;
				case 1:
					// Caso 2
					rotacaoEsq(currentNode, h);
					*h=0;
					break;
			}
		}
	}	
}

void remocaoAVL(AVLtree** root, int32_t x, unsigned short* h){
	AVLtree **currentNode, *sucessor;
	currentNode = root;
	if(*currentNode!=NULL){
		if((*currentNode)->chave==x){
			if((*currentNode)->esq==NULL){
				if((*currentNode)->dir==NULL){
					free(*currentNode);
					*currentNode = NULL;
				}
				else{
					(*currentNode)->chave = (*currentNode)->dir->chave;
					free((*currentNode)->dir);
					(*currentNode)->dir = NULL;
					(*currentNode)->bal = 0;
				}
				*h = 1;
			}
			else if((*currentNode)->dir==NULL){
				(*currentNode)->chave = (*currentNode)->esq->chave;
				free((*currentNode)->esq);
				(*currentNode)->bal = 0;
				*h = 1;
			}
			else{
				sucessor = (*currentNode)->dir;
				while(sucessor->esq != NULL){
					sucessor = sucessor->esq;
				}
				(*currentNode)->chave = sucessor->chave;
				remocaoAVL(&(*currentNode)->dir, sucessor->chave, h);
				
				if(*h){
					switch((*currentNode)->bal){
						case -1:
							rotacaoDir(currentNode, h);
							break;
						case 0:
							(*currentNode)->bal = -1;
							*h = 0;
							break;
						case 1:
							(*currentNode)->bal = 0;
							break;
					}
				}
			}
		}
		else if((*currentNode)->chave>x){
			remocaoAVL(&(*currentNode)->esq, x, h);
			if(*h){
				switch((*currentNode)->bal){
					case -1:
						(*currentNode)->bal = 0;
						break;
					case 0:
						(*currentNode)->bal = 1;
						*h = 0;
						break;
					case 1:
						rotacaoEsq(currentNode, h);
						break;
				}
			}
		}

		if((*currentNode)->chave<x){
			remocaoAVL(&(*currentNode)->dir, x, h);
			if(*h){
				switch((*currentNode)->bal){
					case -1:
						rotacaoDir(currentNode, h);
						break;
					case 0:
						(*currentNode)->bal = -1;
						*h = 0;
						break;
					case 1:
						(*currentNode)->bal = 0;
						break;
				}
			}
		}
	}
}

AVLtree** searchNode(AVLtree** root, int32_t x){
	AVLtree* currentNode, **nodeAdress;
	nodeAdress = root, currentNode = *root;
	if(root!=NULL){
		while((*(nodeAdress)!=NULL)&&((*(nodeAdress))->chave!=x)){
			if(x<currentNode->chave){
				nodeAdress = &currentNode->esq;
				currentNode = currentNode->esq;
			}
			else{
				nodeAdress = &currentNode->dir;
				currentNode = currentNode->dir;
			}
		}
	}
	return nodeAdress;
}

void desalocarAVL(AVLtree** arvore){
	if(*arvore!=NULL){
		if((*arvore)->esq){
			desalocarAVL(&(*arvore)->esq);
		}	
		if((*arvore)->dir){
			desalocarAVL(&(*arvore)->dir);
		}
		free(*arvore);
		*arvore = NULL;
	
	}
}


//cria um no na estrutura certa para ser inserida no arquivo
void createFileNode(AVLtree_arq *AVLstruct, AVLtree* currentNode){
		
	AVLstruct->chave = currentNode->chave;
	AVLstruct->bal = currentNode->bal;
	(currentNode->esq!=NULL) ? (AVLstruct->esq=1):(AVLstruct->esq=0);
	(currentNode->dir!=NULL) ? (AVLstruct->dir=1):(AVLstruct->dir=0);
}



void writeBinFile(FILE* binFile, AVLtree** currentNode){
	AVLtree_arq treeStruct;
	if(currentNode!=NULL){
		createFileNode(&treeStruct, *currentNode);
		
		fwrite(&treeStruct, sizeof(AVLtree_arq), 1, binFile);
		if((*currentNode)->esq!=NULL){
			writeBinFile(binFile, &(*currentNode)->esq);
		}
		if((*currentNode)->dir!=NULL){
			writeBinFile(binFile, &(*currentNode)->dir);
		}

	}
}

void fileToNode(AVLtree **currentNode, AVLtree_arq* treeStruct){
	*currentNode = malloc(sizeof(AVLtree));
	(*currentNode)->chave = treeStruct->chave;
	(*currentNode)->bal = treeStruct->bal;
	(*currentNode)->dir = NULL;
	(*currentNode)->esq = NULL;
}

void readBinFile(FILE* binFile, AVLtree** currentNode){
	AVLtree_arq treeStruct;
	
	fread(&treeStruct, sizeof(AVLtree_arq), 1, binFile);	
	fileToNode(currentNode, &treeStruct);
	if(treeStruct.esq){
		readBinFile(binFile, &(*currentNode)->esq);
	}
	if(treeStruct.dir){
		readBinFile(binFile, &(*currentNode)->dir);
	}
	
}

void drawNode(size_t x, size_t y, int32_t chave, int32_t bal){
	char str[12];
	int width, height;
	sprintf(str, "%d", chave);

	gfx_set_color(255,255,0);
	gfx_filled_rectangle(x-NODE_SIZEX, y-NODE_SIZEY, x+NODE_SIZEX, y+NODE_SIZEY);
	gfx_set_color(255,0,0);

	gfx_rectangle(x-NODE_SIZEX, y-NODE_SIZEY, x+NODE_SIZEX, y+NODE_SIZEY);
	gfx_set_color(0,0,0);

	gfx_set_font_size(14);	
	gfx_get_text_size(str, &width, &height);
	gfx_text(x-width/2, y-height/2-10, str);

	sprintf(str ,"%d", bal);
	gfx_get_text_size(str, &width, &height);
	gfx_text(x-width/2, y-height/2+10, str);
	
}

void gfxCreateTree(AVLtree* currentNode, size_t rate, size_t X, size_t Y, int fatherX){
	if(currentNode!=NULL){
		gfxCreateTree(currentNode->esq, rate/2, X-rate, Y+NODE_INTERVAL_SIZE, X);
		gfxCreateTree(currentNode->dir, rate/2, X+rate, Y+NODE_INTERVAL_SIZE,X);

		if(fatherX>=0){
			gfx_set_color(255, 255, 255);
			gfx_line(fatherX, Y-NODE_INTERVAL_SIZE, X, Y);
		}
		drawNode(X,Y, currentNode->chave, currentNode->bal);
	}
}



int main(){
	AVLtree* root = NULL;
	unsigned short h=0, esc=0;
	int32_t num;
	FILE* binfile;
	char nome[15];

	gfx_init(SCREEN_SIZEX, SCREEN_SIZEY, "Trabalho 4");
	
	do{
		switch(esc){
			case 1:
				while(esc!=0){
					printf("[1] Inserção.\n");
					printf("[2] Remocao.\n");
					printf("[3] Busca\n");
					printf("[4] Desalocar arvore\n");
					printf("[0] Voltar\n");
					printf("Escolha uma das opcoes: ");
					scanf("%hu", &esc);
					if((esc!=0)&&(esc!=1)&&(root==NULL)){
						esc=1;
						printf("Eh necessario um no na arvore antes de realizar as demais operacoes\n");
					}

					switch(esc){
						case 0:
							break;
						case 1:
							printf("Qual numero deseja inserir?: ");
							scanf("%d", &num);
							if(*(searchNode(&root, num))==NULL){
								insertAVL(&root, num, &h);
								h = 0;
								gfx_clear();
								gfxCreateTree(root, SCREEN_SIZEX/4, SCREEN_SIZEX/2, NODE_INTERVAL_SIZE, -1);
								gfx_paint();
								printf("Numero inserido\n");
							}
							else
								printf("Operacao não permitida: Numero encontrado na arvore\n");
							break;
						case 2:
							printf("Qual numero deseja remover?: ");
							scanf("%d", &num);
							if(*(searchNode(&root, num)) != NULL){
								remocaoAVL(&root, num, &h);
								h = 0;
								printf("Operacao de remocao executada com exito\n");
								gfx_clear();
								gfxCreateTree(root, SCREEN_SIZEX/4, SCREEN_SIZEX/2, NODE_INTERVAL_SIZE, -1);
								gfx_paint();
							} 
							else
								printf("operacao invalida\n");
							break;
						case 3:
							printf("Qual numero deseja buscar: ");
							scanf("%d", &num);
							if(*(searchNode(&root, num))!=NULL)
								printf("Numero encontrado na arvore\n");
							else
								printf("Numere nao encontrado na arvore\n");
							break;
						case 4:
							desalocarAVL(&root);
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
					scanf("%hu", &esc);

					switch(esc){
						case 1:
							if(root!=NULL){
								printf("Informe o nome do arquivo\n");
								scanf("%s", nome);
								getchar();
								binfile = fopen(nome, "wb");
								writeBinFile(binfile, &root);
								fclose(binfile);
							}
							else{
								printf("Arvore nao encontrada!!\n");
							}
							break;
						case 2:
							
							printf("digite o nome do arquivo que contem a arvore: ");
							scanf("%s", nome);
							binfile = fopen(nome, "rb");
							if(binfile){	
								if(root!=NULL){
									printf("Arvore ja existe no programa, apagando arvore....\n");
									desalocarAVL(&root);
								}	
								readBinFile(binfile, &root);
								gfx_clear();
								gfxCreateTree(root, SCREEN_SIZEX/4, SCREEN_SIZEX/2, NODE_INTERVAL_SIZE, -1);
								gfx_paint();
								fclose(binfile);
							}
							break;
						case 0:
							break;
						default:
							printf("Opcao invalida\n");
							break;
					}
				}
			break;
		}
		printf("[1] Operacoes na arvore.\n");
		printf("[2] Leitura ou gravacao de arquivo.\n");
		printf("[0] Sair.\n");
		printf("Escolha uma das opcoes: \n");
		scanf("%hu", &esc);
	}while(esc!=0);

	desalocarAVL(&root); // remove a arvore como garantia

}


