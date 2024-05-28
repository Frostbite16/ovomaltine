#include "gfx/gfx.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct listaSim{ //estrutura de uma lista simplismente encadeada
	int data;
	struct listaSim* prox;
}listaSim;

typedef struct listaDup{ //estrutura de uma lista duplamente encadeada
	int data;
	struct listaDup *prox, *ant;
}listaDup;


void insertListSimp(listaSim* h, int t, int esc){ // Aloca uma lista simplismente encadeada, com ou sem no cabeca;
	int i, j=1;
	if(esc==0){
		listaSim* c = h;
		printf("Elemento do %d° no da lista:\n",j);
		scanf("%d",&c->data);
		j++;
	else{
		listaSim* head = malloc(sizeof(int));
		listaSim* c = head;
		h = head;
	}
	

	for(i=0;i<t;i++){
	
		listaSim* p = malloc(sizeof(int));
		printf("Elemento do %d° no da lista:\n",j);
		scanf("%d",&p->data);
		j++;
		c->prox = p;
		c = p;	
	}

	c->next = NULL;
}

void insertListDup(listaDup* head, int t){

	int i;
	listaDup* c = h;
	
	for(i=0;i<t;i++){
	
	
	}
	




}






