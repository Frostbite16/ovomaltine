//#include "\gfx\gfx.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct listaSim{ //estrutura de uma lista simplismente encadeada
	int data;
	struct listaSim* prox;
}listaSim;

typedef struct listaSimPilha{ //estrutura de uma lista simplismente encadeada
	struct listaSimPilha* LastNode;
}Pilha;

typedef struct listaSimFila{ //estrutura de uma lista simplismente encadeada
	struct listaSimFila *FirstNode, *LastNode;
}Fila;

typedef struct listaDup{ //estrutura de uma lista duplamente encadeada
	int data;
	struct listaDup *prox, *ant;
}listaDup;



int AddlistSimOrd(listaSim* h, int x){ // Insercao de uma lista simplismente encadeada e ordenada sem no cabeca
	listaSim *c = malloc(sizeof(listaSim)), *p=h;
	c->data = x;
	if(h=NULL){
		h=c;
		c->prox = NULL;
		return 1;
	}
	while((p->prox->data<=x)&&(p->prox!=NULL)){
		p = p->prox;
	}
	c->prox = p->prox;
	p->prox = c;
	return 1;
}

listaSim* buscaListSimOrd(listaSim* h, int x){
	listaSim *p = h;
	if(h=NULL){
		return NULL;
	}
	while((p->data<x)||(p->prox!=NULL)){
		if(p->data==x){
			return p;
		}
		p = p->prox; 
	}
	if(p->data!=x){
		return NULL;
	}
	return p;
}

int removeListSimOrd(listaSim* h, int x){//TODO
	listaSim *c, *p;
	if(h!=NULL){
		while((p->data<x)||(p->prox!=NULL)){
			if(p->data==x){
				if(p==h){
					h=p->prox;
					delete(p);
					return 1;
				}

			}
		}
		if(p->data!=x){
			return NULL;
		}
	}
	return 0;

}

int insertListDup(listaDup* head, int x){ // Insercao de uma lista duplamente encadeada com no cabeca

	int i;
	listaDup* c = malloc(sizeof(listaDup)), *p=head;
	c->data = x;
	if(head==NULL){
		head = malloc(sizeof(listaDup));
		head->prox = c;
		head->ant = NULL;
		c->prox = NULL;
		c->ant = head;
		return 1;
	}
	
	while(p->prox!=NULL){
		p = p->prox;
	}
	p->prox=c;
	c->prox = NULL;
	c->ant = p;
	return 1;

}






