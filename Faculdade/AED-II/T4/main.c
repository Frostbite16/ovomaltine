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

// Algoritmo para inserção de um no novo na AVL
void insertAVL(AVLtree **raiz, int32_t x, unsigned short *h){
	AVLtree* node, **currentNode, *ptu, *ptv, *aux;
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
					aux = malloc(sizeof(AVLtree));
					aux = *currentNode;
					ptu = (*currentNode)->esq;
					if(ptu->bal==-1){
						aux->esq = ptu->dir;
						ptu->dir = aux;
						aux->bal=0;
						free(*currentNode);
						*currentNode=ptu;
					}
					else{
						ptv = ptu->dir;
						ptu->dir = ptv->esq;
						ptv->esq = ptu;
						aux->esq = ptv->dir;
						ptv->dir = aux;
						(ptv->bal =-1) ? ((*currentNode)->bal=1) : ((*currentNode)->bal=0);
						(ptv->bal = 1) ? (ptu->bal = -1) : (ptu->bal = 0);
						free(*currentNode);
						*currentNode = ptv;
					}
					(*currentNode)->bal = 0, *h=0;
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
					aux = malloc(sizeof(AVLtree));
					aux = *currentNode;
					ptu = aux->dir;

					if(ptu->bal==1){
						aux->dir=ptu->esq;
						ptu->esq = aux;
						aux->bal = 0;
						*currentNode = ptu;
					}
					else{
						ptv = ptu->esq;
						ptu->esq = ptv->dir;
						ptv->dir = ptu;
						aux->dir = ptv->esq;
						ptv->esq = aux;
						(ptv->bal = 1) ? ((*currentNode)->bal = -1) : ((*currentNode)->bal = 0);
						(ptv->bal =-1) ? (ptu->bal = 1) : (ptu->bal=0);
						free(*currentNode);
						*currentNode = ptv;

					}
					(*currentNode)->bal=0, *h=0;
					break;
			}
		}
	}	
}

AVLtree* searchNode(AVLtree** root, int32_t x){
	AVLtree* currentNode = *root;
	while((currentNode!=NULL)&&(currentNode->chave==x)){
		if(currentNode->chave>x)
			currentNode=currentNode->esq;
		if(currentNode->chave<x)
			currentNode=currentNode->dir;
	}	
	return currentNode;
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

void writeBinFile(FILE* binFile, AVLtree* currentNode){
	AVLtree_arq treeStruct;
	if(currentNode!=NULL){
		createFileNode(&treeStruct, currentNode);
		
		fwrite(&treeStruct, sizeof(AVLtree_arq), 1, binFile);
		if(currentNode->esq!=NULL){
			writeBinFile(binFile, currentNode->esq);
		}
		if(currentNode->dir!=NULL){
			writeBinFile(binFile, currentNode->dir);
		}

	}
}

void readBinFile(FILE* binFile, AVLtree** currentNode){
	AVLtree_arq treeStruct;
	fread(&treeStruct, sizeof(AVLtree_arq), 1, binFile);
	if(searchNode(currentNode, treeStruct.chave)==NULL){
		insertAVL(currentNode, treeStruct.chave, 0);
	}
	if(treeStruct.esq==1){
		readBinFile(binFile, currentNode);
	}
	if(treeStruct.dir==1){
		readBinFile(binFile, currentNode);
	}
}

void drawNode(size_t x, size_t y, int32_t chave, int32_t bal){
	char chaveStr[12], balStr[2];
	int width1, height1, width2, height2;
	sprintf(chaveStr, "%d", chave);
	sprintf(balStr, "%d", bal);

	gfx_set_color(255,255,0);
	gfx_filled_rectangle(x-NODE_SIZEX, y-NODE_SIZEY, x+NODE_SIZEX, y+NODE_SIZEY);
	gfx_set_color(255,0,0);

	gfx_rectangle(x-NODE_SIZEX, y-NODE_SIZEY, x+NODE_SIZEX, y+NODE_SIZEY);
	gfx_set_color(80,3,128);

	gfx_set_font_size(15);
	gfx_get_text_size(chaveStr, &width1, &height1);
	gfx_get_text_size(balStr, &width2, &height2);

	gfx_text(x-width1/2, y-height1/2-height2/2, chaveStr);
	gfx_text(x-width2/2, y-height2/2+height2/2, balStr);
	
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
	unsigned short h=0;
	gfx_init(SCREEN_SIZEX, SCREEN_SIZEY, "Trabalho 4");
	insertAVL(&root, 2, &h);
	h=0;
	insertAVL(&root, 4, &h);
	h=0;
	insertAVL(&root, 5, &h);
	h=0;
	insertAVL(&root, 6, &h); 
	gfxCreateTree(root, SCREEN_SIZEX/4, SCREEN_SIZEX/2, NODE_INTERVAL_SIZE, -1);
	gfx_paint();
	getchar();

	desalocarAVL(&root);

}


