//#include "gfx/gfx.h"
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
	if(h==NULL){
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
	if(h==NULL){
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

int removeListSimOrd(listaSim* h, int x){
	listaSim *c = NULL, *p = h;
	if(h!=NULL){
		c = p;
		while((p->prox!=NULL)&&(p->data<=x)){
			c = p;
			p=p->prox;
		}
		if(p->data==x){
			c->prox = p->prox;
			if(p==h){
				h=p->prox;
			}
			free(p);
			return 1;
		}
		return 0;
	}
	return 0;
}



int AddlistSimOrdCirc(listaSim* h, int x){ // Insercao de uma lista simplismente encadeada e ordenada sem no cabeca e circular
	listaSim *c = malloc(sizeof(listaSim)), *p=h;
	c->data = x;
	if(h==NULL){
		h=c;
		c->prox = h;
		return 1;
	}
	while((p->prox->data<=x)&&(p->prox!=h)){
		p = p->prox;
	}
	c->prox = p->prox;
	p->prox = c;
	return 1;
}

listaSim* buscaListSimOrdCirc(listaSim* h, int x){
	listaSim *p = h;
	if(h==NULL){
		return NULL;
	}
	while((p->data<x)||(p->prox!=h)){
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

int removeListSimOrd(listaSim* h, int x){
	listaSim *c = NULL, *p = h;
	if(h!=NULL){
		c = p;
		while((p->prox!=h)&&(p->data<=x)){
			c = p;
			p=p->prox;
		}
		if(p->data==x){
			c->prox = p->prox;
			if(p==h){
				h=p->prox;
			}
			free(p);
			return 1;
		}
		return 0;
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



listaDup* buscaListDup(listaDup* head, int x){
	listaDup* c = head;
	while(c!=NULL){
		if(c->data==x){
			return 1;
		}
		c = c->prox;
	}
	return 0;
}

int buscaListDup(listaDup* head, int x){
	listaDup* c = head;
	while(c!=NULL){
		if(c->data==x){
			c->ant->prox = c->prox;
			if(c->prox!=NULL) c->prox->ant = c->ant;
			free(c);
			return 1;
		}
	}
	return 0;
}






