//#include "gfx/gfx.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct listaSim{ //estrutura de uma lista simplismente encadeada
	int data;
	struct listaSim* prox;
}listaSim;

typedef struct listaSimPilha{ //estrutura de uma Pilha
	struct listaSimPilha* LastNode;
}Pilha;

typedef struct listaSimFila{ //estrutura de uma Fila
	struct listaSimFila *FirstNode, *LastNode;
}Fila;

typedef struct listaDup{ //estrutura de uma lista duplamente encadeada
	int data;
	struct listaDup *prox, *ant;
}listaDup;


/*
	Parametros:
		h: ponteiro pro inicio da lista, Nulo caso não exista
		x: novo valor a ser inserido
	Variaveis:
		c: novo nó criado
		p: ponteiro pro inicio da lista
	Rotina:
		Caso h for nulo:
			h igual ao novo nó c
		caso não:
			itera p até um valor maior ou igual x, ou até o próximo nó for nulo
			c é inserido entre p e o proximo no depois de p
*/
int addlistSimOrd(listaSim* h, int x){ // Insercao de uma lista simplismente encadeada e ordenada sem no cabeca
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


/*
	Parametros:
		h: ponteiro pro inicio da lista, Nulo caso não exista
		x: valor a ser buscado
	Variaveis:
		p: ponteiro pro inicio da lista
	Rotina:
		Caso h for nulo:
			retorna nulo
		caso não:
			itera p até um valor maior ou igual x, ou até o próximo nó for nulo
			se p for igual a x retorna ponteiro de p, caso não retorna nulo
*/
listaSim* buscaListSimOrd(listaSim* h, int x){ // Busca de um elemento em uma lista simplismente encadeada ordenada
	listaSim *p = h;
	if(h==NULL){
		return NULL;
	}
	while((p->data<=x)||(p->prox!=NULL)){
		p = p->prox; 
	}
	if(p->data==x){
		return p;
	}
	return NULL;
}


/*
	Parametros:
		h: ponteiro pro inicio da lista, Nulo caso não exista
		x: valor a ser removido
	Variaveis:
		c: no anterior a p
		p: ponteiro pro inicio da lista
	Rotina:
		Caso h for nulo:
			retorna 0
		caso não:
			itera p até um valor maior ou igual x, ou até o próximo nó for nulo
			iguala c sempre um no anterior a p
			se p for igual a x, o proximo ponteiro de c se torna o proximo ponteiro de p
				se p for igual ao inicio da lista, o ponteiro de h sera igual ao proximo nó depois de p
				desaloca p;
*/
int removeListSimOrd(listaSim* h, int x){ // remoção de um no de uma lista simplismente encadeada ordenada
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
	}
	return 0;
}


/*
	Parametros:
		h: ponteiro pro inicio da lista, Nulo caso não exista
		x: novo valor a ser inserido
	Variaveis:
		c: novo nó criado
		p: ponteiro pro inicio da lista
	Rotina:
		Caso h for nulo:
			h igual ao novo nó c
			proximo no depois de c sera igual a h
		caso não:
			itera p até um valor maior ou igual x, ou até o próximo nó for igual a h
			c é inserido entre p e o proximo no depois de p
*/
int addlistSimOrdCirc(listaSim* h, int x){ // Insercao de um no em uma lista simplismente encadeada e ordenada sem no cabeca e circular
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

/*
	Parametros:
		h: ponteiro pro inicio da lista, Nulo caso não exista
		x: valor a ser buscado
	Variaveis:
		p: ponteiro pro inicio da lista
	Rotina:
		Caso h for nulo:
			retorna nulo
		caso não:
			itera p até um valor maior ou igual x, ou até o próximo nó for igual a h
			se p for igual a x retorna ponteiro de p, caso não retorna nulo
*/
listaSim* buscaListSimOrdCirc(listaSim* h, int x){ // busca de um elemento em uma lista simplismente encadeada ordenada circular
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

/*
	Parametros:
		h: ponteiro pro inicio da lista, Nulo caso não exista
		x: valor a ser removido
	Variaveis:
		c: no anterior a p
		p: ponteiro pro inicio da lista
	Rotina:
		Caso h for nulo:
			retorna 0
		caso não:
			itera p até um valor maior ou igual x, ou até o próximo nó for igual a h
			iguala c sempre um no anterior a p
			se p for igual a x, o proximo ponteiro de c se torna o proximo ponteiro de p
				se p for igual ao inicio da lista
					se o proximo no for igual a h, h vai ser igual a nulo, caso não h vai ser igual ao proximo nó
				desaloca p;
*/
int removeListSimOrdCirc(listaSim* h, int x){ // remoção de um no de uma lista simplismente encadeada ordenada circular
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
				(p->prox!=h) ? (h=p->prox) : (h=NULL);
			}
			free(p);
			return 1;
		}
	}
	return 0;
}
/*
	Parametros:
		head: ponteiro pro no cabeça, Nulo caso não exista
		x: novo valor a ser inserido
	Variaveis:
		c: novo nó criado
		p: ponteiro pro nó cabeça
	Rotina:
		Caso head for nulo:
			cria um novo no cabeça e aponta o proximo no para c e o anterior para nulo
			aponta no anterior a c para o no cabeça
		caso não:
			itera p até um valor nulo
			c é inserido depois de p
			o ponteiro anterior de c e igualado a p
*/
int insertListDup(listaDup* head, int x){ // Insercao de uma lista duplamente encadeada com no cabeca

	int i;
	listaDup* c = malloc(sizeof(listaDup)), *p=head;
	c->data = x;
	c->prox = NULL;
	if(head==NULL){
		head = malloc(sizeof(listaDup));
		head->prox = c;
		head->ant = NULL;
		c->ant = head;
		return 1;
	}
	
	while(p->prox!=NULL){
		p = p->prox;
	}
	p->prox=c;
	c->ant = p;
	return 1;
}


/*
	Parametros:
		head: ponteiro pro inicio da lista, Nulo caso não exista
		x: valor a ser buscado
	Variaveis:
		c: ponteiro pro inicio da lista
	Rotina:
		Caso c for nulo:
			retorna nulo
		caso não:
			itera p até um valor nulo
			se c for igual a x retorna c, caso não retorna nulo
*/
listaDup* buscaListDup(listaDup* head, int x){ // busca de um valor de uma lista duplamente encadeada
	listaDup* c = head;
	while(c!=NULL){
		
			if(c->data==x){
				return c;
			}
			c = c->prox;
	}
	return NULL;
}

/*
	Parametros:
		head: ponteiro pro inicio da lista, Nulo caso não exista
		x: valor a ser buscado
	Variaveis:
		c: ponteiro pro inicio da lista
	Rotina:
		Caso c for nulo:
			retorna 0
		caso não:
			itera c até um valor nulo
			se c for igual a x remove o ponteiro e aponta o ponteiro do no anterior para o proximo
			e caso o proximo não for nulo aponta o ponteiro anterior do proximo para o anterior a c
			desaloca c
*/
int removeListDup(listaDup* head, int x){ // Remoção de um nó de uma lista duplamente encadeada
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

/*
	Parametros:
		stack: estrutura de pilha
		x: novo valor a ser inserido
	Variaveis:
		c: novo nó criado
		p: ponteiro pro fim da lista
	Rotina:
		Caso h for nulo:
			iguala o fim da lista a c;
		caso não:
			coloca o proximo nó da lista como c
			coloca o ultimo no da lista como c
			
*/
int addStack(Pilha stack, int x){ // Insercao de um nó em uma pilha
	listaSim *c = malloc(sizeof(listaSim)), *p=stack.LastNode;
	c->data = x;
	if(p==NULL){
		stack.LastNode = c;
		c->prox = NULL;
		return 1;
	}
	
	c->prox = p;
	stack.LastNode = c;
	return 1;
}

/*
	Parametros:
		stack: estrutura de pilha
	Variaveis:
		p: ponteiro pro fim da lista
	Rotina:
		Caso h for nulo:
			retorna 0
		caso não:
			iguala o ultimo da lista ao anterior a p
			desaloca p
			
*/
int removeStack(Pilha stack){ // remoção de um nó em uma pilha
	listaSim *p = stack.LastNode;
	if(p!=NULL){
		stack.LastNode=p->prox;
		free(p);
		return 1;
	}
	return 0;
}

/*
	Parametros:
		queue: estrutura de fila
		x: novo valor a ser inserido
	Variaveis:
		c: novo nó criado
		p: ponteiro pro fim da lista
	Rotina:
		Caso h for nulo:
			iguala o fim e o inicio da lista a c;
		caso não:
			coloca o proximo e o ultimo nó da lista como c
			
*/
int addQueue(Fila queue, int x){ // Insercao de um no em uma fila
	listaSim *c = malloc(sizeof(listaSim)), *p=queue.LastNode;
	c->data = x;
	c->prox= NULL;
	if(p==NULL){
		queue.LastNode=c;
		queue.FirstNode=c;
		return 1;
	}

	p->prox = c;
	queue.LastNode=c;
	return 1;
}

/*
	Parametros:
		queue: estrutura de pilha
	Variaveis:
		p: ponteiro pro fim da lista
	Rotina:
		Caso h for nulo:
			retorna 0
		caso não:
			iguala o primeiro elemento da lista ao no anterior a p
			desaloca p
			
*/
int removeQueue(Fila queue){ // remoção de um nó em uma fila
	listaSim* p = queue.FirstNode;
	if(p!=NULL){
		queue.FirstNode=p->prox;
		free(p);
		return 1;
	}
	return 0;
}







