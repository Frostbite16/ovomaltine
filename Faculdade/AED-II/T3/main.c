#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "gfx/gfx.h"
#define SCREEN_SIZEX 1000
#define SCREEN_SIZEY 800
#define NODE_SIZEX 17
#define NODE_SIZEY 14
#define NODE_INTERVAL_SIZE 50
#define OFFSET 5



typedef struct SearchTree{
    int data;
    struct SearchTree *leftChild, *rightChild;
}searchTreeNode;

typedef struct s_arq_no{
    __int32_t chave:30;
    __uint32_t esq:1;
    __uint32_t dir:1;

}sTreeNode_arq;


int insertNode(searchTreeNode** nodeAdress, int x){
    searchTreeNode *newChild = malloc(sizeof(searchTreeNode)), *currentChild;
    newChild->data = x;
    newChild->leftChild = NULL; 
    newChild->leftChild = NULL;
    if(*(nodeAdress)==NULL){
        *(nodeAdress) = newChild;
        return 1;
    }
    while((*(nodeAdress)!=NULL)){
        currentChild =*(nodeAdress);
        if(x<currentChild->data){
            nodeAdress=&currentChild->leftChild;
        }
        else{
            nodeAdress=&currentChild->rightChild;
        }
    }
    *nodeAdress = newChild;
    return 1;
}

searchTreeNode* searchMax(searchTreeNode* currentNode){
    if(currentNode!=NULL){
        while(currentNode->rightChild!=NULL){
            currentNode = currentNode->rightChild;
        }
    }
    return currentNode;
}

searchTreeNode* searchMin(searchTreeNode* currentNode){
    if(currentNode!=NULL){
        while(currentNode->leftChild!=NULL){
            currentNode = currentNode->leftChild;
        }
    }
    return currentNode;
}

searchTreeNode** searchNode(searchTreeNode* root, int x){
    searchTreeNode *currentNode = root, **nodeAdress=&root;
    if(root!=NULL){
        while((currentNode!=NULL)&&(currentNode->data!=x)){
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
    return nodeAdress;
}



searchTreeNode** searchSucessor(searchTreeNode* root, int x ){
    searchTreeNode *currentNode = root, **sucessor = NULL;
    (root->data>x) ? (sucessor=&root) : (sucessor=NULL);
    if(currentNode!=NULL){
        while((currentNode!=NULL)&&(currentNode->data!=x)){
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
		if((currentNode!=NULL)&&(currentNode->rightChild!=NULL)){
			sucessor = &currentNode->rightChild;
            currentNode=currentNode->rightChild;
            while(currentNode->leftChild!=NULL){
                sucessor = &currentNode->leftChild;
                currentNode=currentNode->leftChild;
            }
            return sucessor;
		}
		else if(currentNode==NULL)
			return NULL;
    }
    return sucessor;
}

searchTreeNode** searchAntecessor(searchTreeNode* root, int x ){
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



int removeNode(searchTreeNode** nodeAdress, int x){
    searchTreeNode *currentNode, *root = *(nodeAdress), *sucessor, **adressToSucessor;
    nodeAdress = searchNode(root, x);
    if(*(nodeAdress)!=NULL){
            currentNode = *(nodeAdress);
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

            adressToSucessor = searchSucessor(root, x);
			sucessor = *(adressToSucessor);           
			if(currentNode->rightChild==sucessor){
				*(nodeAdress)=sucessor;
				sucessor->leftChild=currentNode->leftChild;
				free(currentNode);
				return 1;
			}
			*(adressToSucessor) = sucessor->rightChild;
			currentNode->data=sucessor->data;
			free(sucessor);
			return 1;
					

    }
    return 0;
}

void freeTree(searchTreeNode **adressToNode){
    searchTreeNode *currentNode = *(adressToNode);
    if(currentNode->leftChild!=NULL){
        freeTree(&currentNode->leftChild);
    }
    if(currentNode->rightChild!=NULL){
        freeTree(&currentNode->rightChild);
    }
    free(currentNode);
	currentNode = NULL;

}

void createFileNode(sTreeNode_arq *treeStruct, searchTreeNode* currentNode){
   
		treeStruct->chave = currentNode->data;
		(currentNode->leftChild!=NULL) ? (treeStruct->esq=1):(treeStruct->esq=0);
	   	(currentNode->rightChild!=NULL) ? (treeStruct->dir=1):(treeStruct->dir=0);	

}

FILE* openBinFile(char* fileName, char* mode){
	if(access(fileName, F_OK)==0){
		return fopen(fileName, mode);
	}
	return NULL;
} 

void writeBinFile(FILE* binFile, searchTreeNode* currentNode){
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

void readBinFile(FILE* binFile, searchTreeNode** currentNode){
	sTreeNode_arq treeStruct;
	fread(&treeStruct, sizeof(sTreeNode_arq), 1, binFile);
	if(*(searchNode(*(currentNode), treeStruct.chave)) == NULL){
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




void drawNode(unsigned x, unsigned y, int chave){
    char txt[12];
	int width, height;
    sprintf(txt, "%d", chave); 
		
	gfx_set_color(255,255,0);
    gfx_filled_rectangle(x-NODE_SIZEX, y-NODE_SIZEY, x+NODE_SIZEX, y+NODE_SIZEY);

	gfx_set_color(255,0,0);

	gfx_rectangle(x-NODE_SIZEX, y-NODE_SIZEY, x+NODE_SIZEX, y+NODE_SIZEY);

    gfx_set_color(80,3,128);

	
	
	gfx_set_font_size(15);
	gfx_get_text_size(txt, &width, &height);
	
    gfx_text(x-width/2,y-height/2,txt);

		

}

void gfxCreateTree(searchTreeNode* currentNode, unsigned rate, unsigned X, unsigned Y, int fatherX){
	
	
	if(currentNode!=NULL){   		
        
		gfxCreateTree(currentNode->leftChild, rate/2 , X  - rate + OFFSET, Y + NODE_INTERVAL_SIZE, X);
        gfxCreateTree(currentNode->rightChild, rate/2, X  + rate - OFFSET, Y + NODE_INTERVAL_SIZE, X);
  		
	  	if(fatherX>=0){
			gfx_set_color(255, 255, 255);
			gfx_line(fatherX, Y - NODE_INTERVAL_SIZE, X, Y);
		}
	
	    drawNode(X,Y,currentNode->data);
	}
    

}


int main(){

   /* searchTreeNode *root = NULL, *temp = NULL;
    
	FILE* binFile = openBinFile("binfile.bin", "wb");
    insertNode(&root, 10);
    insertNode(&root, 9);
    insertNode(&root, 8);
    insertNode(&root, 7);   
    insertNode(&root, 50);
    insertNode(&root, 55);
    insertNode(&root, 60);
    insertNode(&root, 30);
    insertNode(&root, 20);
    insertNode(&root, 35);
	insertNode(&root, 80);
	insertNode(&root, 95);
	writeBinFile(binFile, root);
	fclose(binFile);
	binFile = openBinFile("binfile.bin", "rb");
    readBinFile(binFile, &temp);

    gfx_init(SCREEN_SIZEX, SCREEN_SIZEY, "Trabalho 3");
    gfxCreateTree(root, SCREEN_SIZEX/4,SCREEN_SIZEX/2, NODE_INTERVAL_SIZE, -1);
    gfx_paint();
    sleep(5);
    gfx_quit();

  

    freeTree(&root);
	freeTree(&temp);
    
    return 0;*/

	searchTreeNode *root = NULL;
	unsigned short esc=0;
	int num;


	do{
		
		
		switch(esc){
		
			case 1:				
				printf("[1] Inserção.\n");
				printf("[2] Remocao.\n");
				printf("[3] Busca pelo sucessor.\n");
				printf("[4] Busca pelo antecessor.\n");
				printf("[5] Busca pelo maior no da arvore.\n");
				printf("[6] Busca pelo menor no da arvore.\n");
				printf("[7] Desalocar arvore\n");
				printf("[8] Sair");
				printf("[0] Voltar.\n");
				printf("Escolha uma das opcoes: \n");
				scanf("%hu", &esc);
				switch(esc){
					case 1:
						printf("Qual numero deseja inserir?: \n");
						scanf("%d", &num);
						if(*(searchNode(root, num))==NULL){
							insertNode(&root, num);	
						}
						else{
							printf("Operacao nao permitida: Numero encontrado na arvore");
						}
					break;
					case 2:			
					break;	
				
				
				
				
				}
			break;			
		
		}
	
		printf("[1] Operacoes na arvore.\n");
		printf("[2] Leitura ou gravacao de arquivo.\n");
		printf("[0] Sair.\n");
		printf("Escolha uma das opcoes: \n");			
		scanf("%hu", &esc);

		
	
	
	}while(esc!=0);


	
	








}

/*
 *	
 *	
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
  */
