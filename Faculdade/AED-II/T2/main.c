#include "gfx/gfx.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SCREEN_SIZEX 800
#define SCREEN_SIZEY 600
#define OFFSET 50
#define NODE_SIZEX 50
#define NODE_SIZEY 20
#define ARROW_HEIGHT 10

typedef struct listaSim{ //estrutura de uma lista simplismente encadeada
	int data;
	struct listaSim* prox;
}listaSim;

typedef struct listaSimPilha{ //estrutura de uma Pilha
	struct listaSim* LastNode;
}Pilha;

typedef struct listaSimFila{ //estrutura de uma Fila
	struct listaSim* FirstNode, *LastNode;
}Fila;

typedef struct listaDup{ //estrutura de uma lista duplamente encadeada
	int data;
	struct listaDup *prox, *ant;
}listaDup;

/*
	Parametros:
		esc: apaga uma parte da tela caso 1, e toda tela caso contrario
		x1,y1,x2,y2: pontos
	Rotina:
		pinta parte ou toda tela com um quadrado preto
*/
void cleanScreen(int esc, int x1, int y1, int x2, int y2){

        if(esc==1){
                gfx_set_color(0,0,0);
                gfx_filled_rectangle(x1, y1, x2, y2);

        }
        else{
                gfx_set_color(0,0,0);
                gfx_filled_rectangle(0,0, SCREEN_SIZEX, SCREEN_SIZEY);
        }
		gfx_paint();

}
/*
	Parametros:
		x1,y1: pontos
		text: texto para ser escrito
		esc: modo

	Rotina:
		corrige o valor de y1 baseado no tamanho de cada quadrado da lista
		Caso esc for 1:
			desenha o texto na tela
		caso 2:
			apaga o texto da tela
*/

void updateLabel(int x1, int y1, char* text, int esc){
		y1-=NODE_SIZEY;
        switch(esc){
            case 1:

                gfx_set_color(255,255,255);
                gfx_text(x1+10,y1+10, text);
                break;

            case 2:
				cleanScreen(1,x1,y1,SCREEN_SIZEX+60,y1+60);
            	break;

           	default:
				break;
		}
		gfx_paint();

}

/*
	Parametros:
		x1,y1,x2,y2: pontos na tela
		mode : modo
	Rotina:
		Caso mode for 1:
			desenha uma flecha para direita
		caso não:
			desenha uma flecha para a esquerda
*/
void drawArrow(int x1,int y1,int x2,int y2, int mode){

	gfx_line(x1,y1,x2,y2);
	if(mode==1){
		gfx_line(x2,y2, x2-ARROW_HEIGHT, y2-5);
		gfx_line(x2,y2, x2-ARROW_HEIGHT, y2+5);
	}
	else{
		gfx_line(x1,y1, x1+ARROW_HEIGHT, y1-5);
		gfx_line(x1,y1, x1+ARROW_HEIGHT, y1+5);
	}

}

/*
	Parametros:
		x1,y1,x2,y2: pontos na tela
		data: conteudo da lista
		mode: modo
	Variaveis:
		txt: buffer dos dados
		num: quantidade de numeros
	Rotina:
		Caso mode for 1:
			desenha parte da lista com apenas um ponteiro para frente
		caso mode for 2:
			desenha parte da lista com um ponteiro para frente e para tras
		caso mode for 2:
			desenha parte da lista com um ponteiro somente para tras
*/
void drawNode(int x1, int y1, int x2, int y2, int data, int mode){

	char txt[12];
	int num = sprintf(txt, "%d", data)-1;
	switch(mode){
		case 1:
			gfx_rectangle(x1,y1,x2,y2);
			gfx_text(x1+(x2-x1)/2-11-num*2, y1+(y2-y1)/2-10, txt);
			gfx_rectangle(x2-10,y1,x2,y2);
			break;
		case 2:
			gfx_rectangle(x1,y1,x2,y2);
			gfx_text(x1+(x2-x1)/2-num*5-2, y1+(y2-y1)/2-10, txt);
			gfx_rectangle(x2-10,y1,x2,y2);
			gfx_rectangle(x1,y1,x1+10,y2);
			break;
		case 3:
			gfx_rectangle(x1,y1,x2,y2);
			gfx_text(x1+(x2-x1)/2-2-num*2, y1+(y2-y1)/2-10, txt);
			gfx_rectangle(x1,y1,x1+10,y2);
	}

}

/*
	Parametros:
		x1,y1,x2,y2: pontos na tela
	Rotina:
		desenha o simbolo lambda
*/
void drawLambda(int x1, int y1, int x2, int y2){
	gfx_line(x1-6,y1-16, x2+6,y2+16);
	gfx_line(x1-2,y1-2, x2-9,y2+16);
	//gfx_line(x1,y1, x2-6,y2-10);
}

/*
	Parametros:
		h: ponteiro para a lista
		y: local no eixo y onde a lista deve ser inserida
		sizeX: largura dos retangulos que representam a lista
		sizeY: altura dos retangulos que representam a lista
	Rotina:
		apaga o espaço onde a lista deve ser inserida e desenha a lista até o elemento final
		caso a lista for vazia não desenha nada e apaga o texto
*/
int drawLinkedList(listaSim *h, int y, int sizeX, int sizeY){
    listaSim *p = h;

    int x1=40, x2=x1+sizeX, y1=y-sizeY, y2=y+sizeY;
	cleanScreen(1,0,y-sizeY-10,800,y+sizeY+10);
    gfx_set_color(255,255,255);
    if(h!=NULL){

		drawArrow(x1-15,y,x1,y,1);

		while((p->prox!=NULL)&&(p->prox!=h)){

			drawNode(x1,y1,x2,y2,p->data,1);
			drawArrow(x2,y,x2+40,y,1);
            x1=x2+40;
            x2=x1+sizeX;
            p=p->prox;
        }

		drawNode(x1,y1,x2,y2,p->data,1);
		drawArrow(x2,y,x2+40,y,1);

        x1=x2+40;
		x2=x1+sizeX;

		if(p->prox==NULL){
			x1+=16;
			drawLambda(x1,y,x1,y);
		}
		else{
			x1+=16;
			gfx_line(x1, y, x1+20,y);	
		}	
    }
    else{
		updateLabel(0,y-40,"a",2);
    }
	gfx_paint();
	return 1;
}

/*
	Parametros:
		h: ponteiro para a lista
		y: local no eixo y onde a lista deve ser inserida
		sizeX: largura dos retangulos que representam a lista
		sizeY: altura dos retangulos que representam a lista
	Rotina:
		apaga o espaço onde a lista deve ser inserida e desenha a lista até o elemento final
		caso a lista for vazia desenha apenas o nó cabeça
		caso mode for 1 apaga o no cabeça e o texto
*/
int drawDoubleLinkedList(listaDup *h, int y, int sizeX, int sizeY, int mode){
	listaDup *p = h;
	int x1=40, x2=x1+sizeX, y1=y-sizeY, y2=y+sizeY;
	cleanScreen(1,10,y-sizeY-10,800,y+sizeY+10);
	gfx_set_color(255,255,255);
	
	if(mode!=1){
		drawLambda(x1,y,x1,y);
		x1+=16;

		drawArrow(x1,y,x1+40,y,2);

		x1+=40;
		x2 = x1+sizeX;
		drawNode(x1,y1,x2,y2,0x0,2);
		drawArrow(x2,y1+(y2-y1)/3,x2+40,y1+(y2-y1)/3,1);
		drawArrow(x2,y1+2*(y2-y1)/3,x2+40,y1+2*(y2-y1)/3,2);
		x1=x2+40;
		x2=x1+sizeX;
	}
	
	if(h!=NULL){

		while(p!=NULL){

			drawNode(x1,y1,x2,y2,p->data,2);
			drawArrow(x2,y1+(y2-y1)/3,x2+40,y1+(y2-y1)/3,1);
			drawArrow(x2,y1+2*(y2-y1)/3,x2+40,y1+2*(y2-y1)/3,2);

			x1=x2+40;
			x2=x1+sizeX;
			p=p->prox;
		}
		x1+=16;
		drawLambda(x1,y,x1,y);

	}
	if(mode==1){
		updateLabel(0,y-40,"a",2);
	}
	
	gfx_paint();
	return 1;
}

/*
	Parametros:
		h: ponteiro para a lista
		y: local no eixo y onde a lista deve ser inserida
		sizeX: largura dos retangulos que representam a lista
		sizeY: altura dos retangulos que representam a lista
	Rotina:
		apaga o espaço onde a lista deve ser inserida e desenha a lista até o elemento final
		caso a lista for vazia não desenha nada e apaga o texto
*/
void drawStackorQueue(Pilha* stack, Fila* queue, int y, int sizeX, int sizeY, int mode){
	listaSim* p=NULL;
	int x1=SCREEN_SIZEX-40, x2=x1-sizeX, y1=y-sizeY, y2=y+sizeY;
	cleanScreen(1,10,y-sizeY-10,800,y+sizeY+10);
	gfx_set_color(255,255,255);

	if((stack!=NULL)||(queue!=NULL)){
		if(mode==1){
			p = stack->LastNode;
		}
		else{
			p = queue->FirstNode;
		}
		x1-=40;
		x2=x1-sizeX;
		drawArrow(x1,y,x1+40,y,2);
		

		while((p->prox!=NULL)){

			drawNode(x2,y1,x1,y2,p->data,3);
			drawArrow(x2-40,y,x2,y,2);
            x1=x2-40;
            x2=x1-sizeX;
            p=p->prox;
        }
		drawNode(x2,y1,x1,y2,p->data,3);
		drawArrow(x2-40,y,x2,y,2);

		x2-=56;
		drawLambda(x2,y,x2,y);
	}
	else{
		updateLabel(0,y-40,"a",2);
    }
	gfx_paint();
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
		caso não:
			itera p até um valor maior ou igual x, ou até o próximo nó for nulo
			c é inserido entre p e o proximo no depois de p
*/
listaSim *addlistSimOrd(listaSim* h, int x){ // Insercao de uma lista simplismente encadeada e ordenada sem no cabeca
	listaSim *c = malloc(sizeof(listaSim)), *p=h; 
	c->data = x;
	if((h==NULL)||(h->data>=x)){
		c->prox = h;
		h=c;
		drawLinkedList(h,SCREEN_SIZEY/6-OFFSET+10, NODE_SIZEX, NODE_SIZEY);
		updateLabel(0,SCREEN_SIZEY/6-OFFSET-30,"Lista Simplismente Encadeada Ordenada",1);
		return h;
	}
	while((p->prox!=NULL)&&(p->prox->data<x)){
		p = p->prox;
	}
	c->prox = p->prox;
	p->prox = c;
	drawLinkedList(h,SCREEN_SIZEY/6-OFFSET+10, NODE_SIZEX, NODE_SIZEY);
	return h;
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
	if((h==NULL)){
		return NULL;
	}
	while((p->prox!=NULL)&&(p->prox->data<=x)){
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
listaSim* removeListSimOrd(listaSim* h, int x){ // remoção de um no de uma lista simplismente encadeada ordenada
	listaSim *c = NULL, *p = h;
	if(h!=NULL){
		c = p;
		while((p->prox!=NULL)&&(p->data<x)){
			c = p;
			p=p->prox;
		}
		if(p->data==x){
			c->prox = p->prox;
			if(p==h){
				h=p->prox;
			}
			free(p);
			drawLinkedList(h,SCREEN_SIZEY/6-OFFSET+10, NODE_SIZEX, NODE_SIZEY);
			return h;
		}
	}
	return h;
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
listaSim* addlistSimOrdCirc(listaSim* h, int x){ // Insercao de um no em uma lista simplismente encadeada e ordenada sem no cabeca e circular
	listaSim *c = malloc(sizeof(listaSim)), *p=h;
	c->data = x;
	if(h==NULL){
		h=c;
		c->prox = h;
		updateLabel(0,2*SCREEN_SIZEY/6-OFFSET-30,"Lista Simplismente Encadeada Ordenada Circular",1);
		drawLinkedList(h,2*SCREEN_SIZEY/6-OFFSET+10, NODE_SIZEX, NODE_SIZEY);
		return h;
	}
	else if(h->data>=x){
		c->prox = h;
		while(p->prox!=h){
			p = p->prox;
		}
		h=c;
		p->prox=h;
		drawLinkedList(h,2*SCREEN_SIZEY/6-OFFSET+10, NODE_SIZEX, NODE_SIZEY);
		return h;
	}
	while((p->prox!=h)&&(p->prox->data<=x)){
		p = p->prox;
	}
	c->prox = p->prox;
	p->prox = c;
	drawLinkedList(h,2*SCREEN_SIZEY/6-OFFSET+10, NODE_SIZEX, NODE_SIZEY);
	return h;
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
	while((p->prox!=h)&&(p->prox->data<=x)){
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
listaSim* removeListSimOrdCirc(listaSim* h, int x){ // remoção de um no de uma lista simplismente encadeada ordenada circular
	listaSim *c = NULL, *p = h;
	if(h!=NULL){
		c = p;
		while((p->prox!=h)&&(p->data<x)){
			c = p;
			p=p->prox;
		}
		if(p->data==x){
			c->prox = p->prox;
			if(p==h){
				if(p->prox!=h){ 
					h=p->prox;
					c=p->prox;
					while(c->prox!=p){
						c=c->prox;
					} 
					c->prox=h;
				} 
				else{
					(h=NULL);
				}
			}
			free(p);
			drawLinkedList(h,2*SCREEN_SIZEY/6-OFFSET+10, NODE_SIZEX, NODE_SIZEY);
			return h;
		}
	}
	return h;
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
listaDup* addListDup(listaDup* head, int x){ // Insercao de uma lista duplamente encadeada com no cabeca

	listaDup* c = malloc(sizeof(listaDup)), *p=head;
	c->data = x;
	c->prox = NULL;
	if(head==NULL){
		head = malloc(sizeof(listaDup));
		head->prox = c;
		head->ant = NULL;
		c->ant = head;
		updateLabel(0,3*SCREEN_SIZEY/6-OFFSET-30,"Lista Duplamente Encadeada",1);
		drawDoubleLinkedList(head->prox,3*SCREEN_SIZEY/6-OFFSET+10,NODE_SIZEX,NODE_SIZEY,0);
		return head;
		
	}
	
	while(p->prox!=NULL){
		p = p->prox;
	}
	p->prox=c;
	c->ant = p;
	drawDoubleLinkedList(head->prox,3*SCREEN_SIZEY/6-OFFSET+10,NODE_SIZEX,NODE_SIZEY,0);
	return head;
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
			retorna c
		caso não:
			itera c até um valor nulo
			se c for igual a x remove o ponteiro e aponta o ponteiro do no anterior para o proximo
			e caso o proximo não for nulo aponta o ponteiro anterior do proximo para o anterior a c
			desaloca c
*/


listaDup* removeListDup(listaDup* head, int x){ // Remoção de um nó de uma lista duplamente encadeada
	listaDup* c = head->prox;
	while(c!=NULL){
		if(c->data==x){
			c->ant->prox = c->prox;
			if(c->prox!=NULL) c->prox->ant = c->ant;
			free(c);
			drawDoubleLinkedList(head->prox,3*SCREEN_SIZEY/6-OFFSET+10,NODE_SIZEX,NODE_SIZEY,0);
			return head;
		}
		c=c->prox;
	}
	return head;
}

/*
	Parametros:
		head: ponteiro pro inicio da lista, Nulo caso não exista
	Rotina:
		Caso head for nulo:
			retorna ponteiro pro no cabeça
		caso não:
			desaloca nó cabeça e iguala ponteiro head para NULL;
*/
listaDup* removeHead(listaDup* head){
	if(head!=NULL){
		while(head->prox!=NULL){
			removeListDup(head, head->prox->data);
		}
		free(head);
		head=NULL;
		drawDoubleLinkedList(head,3*SCREEN_SIZEY/6-OFFSET+10,NODE_SIZEX,NODE_SIZEY,1);
	}
	return head;
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
Pilha* addStack(Pilha *stack, int x){ // Insercao de um nó em uma pilha
	listaSim *c = malloc(sizeof(listaSim)), *p=NULL;
	c->data = x;
	if(stack==NULL){
		stack = malloc(sizeof(stack));
		stack->LastNode = c;
		c->prox = NULL;
		updateLabel(0,4*SCREEN_SIZEY/6-OFFSET-30,"Pilha Encadeada",1);
		drawStackorQueue(stack,NULL,4*SCREEN_SIZEY/6-OFFSET+10,NODE_SIZEX, NODE_SIZEY,1);
		return stack;
	}
	p=stack->LastNode;
	c->prox = p;
	stack->LastNode = c;
	drawStackorQueue(stack,NULL,4*SCREEN_SIZEY/6-OFFSET+10,NODE_SIZEX, NODE_SIZEY,1);
	return stack;
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
Pilha* removeStack(Pilha *stack){ // remoção de um nó em uma pilha
	listaSim *p = NULL;
	if(stack!=NULL){
		p = stack->LastNode;
		stack->LastNode=p->prox;
		if(stack->LastNode==NULL){
			free(stack);
			stack=NULL;
		}
		free(p);
		drawStackorQueue(stack,NULL,4*SCREEN_SIZEY/6-OFFSET+10,NODE_SIZEX, NODE_SIZEY,1);
	}
	return stack;
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
Fila* addQueue(Fila *queue, int x){ // Insercao de um no em uma fila
	listaSim *c = malloc(sizeof(listaSim)), *p=NULL;
	c->data = x;
	c->prox= NULL;
	if(queue==NULL){
		queue = malloc(sizeof(queue));
		queue->LastNode=c;
		queue->FirstNode=c;
		drawStackorQueue(NULL,queue,5*SCREEN_SIZEY/6-OFFSET+10,NODE_SIZEX, NODE_SIZEY,2);
		updateLabel(0,5*SCREEN_SIZEY/6-OFFSET-30,"Fila Encadeada",1);
		return queue;
	}
	p=queue->LastNode;
	p->prox = c;
	queue->LastNode=c;
	drawStackorQueue(NULL,queue,5*SCREEN_SIZEY/6-OFFSET+10,NODE_SIZEX, NODE_SIZEY,2);
	return queue;
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
Fila* removeQueue(Fila *queue){ // remoção de um nó em uma fila
	listaSim* p = NULL;
	if(queue!=NULL){
		p = queue->FirstNode;
		queue->FirstNode=p->prox;
		if(queue->FirstNode==NULL){
			free(queue);
			queue=NULL;
		}
		free(p);
		drawStackorQueue(NULL,queue,5*SCREEN_SIZEY/6-OFFSET+10,NODE_SIZEX, NODE_SIZEY,2);

	}
	return queue;
}



int main(){
	
	// variaveis para estrutura de dados
	listaSim *h = NULL, *c = NULL;
	Pilha *p = NULL;
	Fila *f = NULL;
	listaDup *head = NULL;

	int esc=1,num=1;

	gfx_init(SCREEN_SIZEX, SCREEN_SIZEY, "Estruturas de dados");//Inicia a tela

	while(esc!=0){
		
		system("clear");
		printf("Desenhar Estruturas de dados\nEscolha Uma opcao\n");
		printf("(1) Lista Simplismente Encadeada Ordenada\n(2) Lista Simplismente Encadeada Ordenada Circular\n");
		printf("(3) Lista Duplamente Encadeada\n(4) Pilha Encadeada\n(5) Fila Encadeada\n(0) Sair\n");
	
		scanf("%d",&esc);
		switch(esc){
			case 1:
				while(esc!=4){
					printf("Qual operacao deseja\n");
					printf("(1) Inserir\n(2) Busca\n(3) Remocao\n(4) Voltar\n");	
					scanf("%d",&esc);
					switch(esc){

						case 1:
							printf("Qual numero deseja Inserir?: ");
							scanf("%d",&num);
							h = addlistSimOrd(h,num);
							break;
						case 2:
							printf("Qual numero deseja encontrar?: ");
							scanf("%d",&num);
							if(buscaListSimOrd(h,num)!=NULL){
								printf("Numero %d encontrado\n",num);
							}
							else{
								printf("numero não encontrado\n");
							}
							break;
						case 3:
							printf("Qual numero deseja remover?: ");
							scanf("%d",&num);
							if(buscaListSimOrd(h,num)!=NULL){
								h=removeListSimOrd(h,num);
								printf("Numero removido\n");
							}
							else{
								printf("Numero nao encontrado\n");
							}
							break;
						default:
							break;
					}

				}
				esc=1;
				break;
			case 2:
				while(esc!=4){
					printf("Qual operacao deseja\n");
					printf("(1) Inserir\n(2) Busca\n(3) Remocao\n(4) Voltar\n");	
					scanf("%d",&esc);
					switch(esc){

						case 1:
							printf("Qual numero deseja Inserir?: ");
							scanf("%d",&num);
							c = addlistSimOrdCirc(c,num);
							break;
						case 2:
							printf("Qual numero deseja encontrar?: ");
							scanf("%d",&num);
							if(buscaListSimOrdCirc(c,num)!=NULL){
								printf("Numero %d encontrado\n",num);
							}
							else{
								printf("numero não encontrado\n");
							}
							break;
						case 3:
							printf("Qual numero deseja remover?: ");
							scanf("%d",&num);
							if(buscaListSimOrdCirc(c,num)!=NULL){
								c=removeListSimOrdCirc(c,num);
								printf("Numero removido\n");
							}
							else{
								printf("Numero nao encontrado\n");
							}
							break;
						default:
							break;
					}

				}
				esc=2;
				break;
			case 3:
				while(esc!=5){
					printf("Qual operacao deseja\n");
					printf("(1) Inserir\n(2) Busca\n(3) Remocao\n(4) Excluir no cabeca\n(5) Voltar\n");	
					scanf("%d",&esc);
					switch(esc){

						case 1:
							printf("Qual numero deseja Inserir?: ");
							scanf("%d",&num);
							head = addListDup(head,num);
							break;
						case 2:
							printf("Qual numero deseja encontrar?: ");
							scanf("%d",&num);
							if(buscaListDup(head,num)!=NULL){
								printf("Numero %d encontrado\n",num);
							}
							else{
								printf("numero não encontrado\n");
							}
							break;
						case 3:
							printf("Qual numero deseja remover?: ");
							scanf("%d",&num);
							if(buscaListDup(head,num)!=NULL){
								head=removeListDup(head,num);
								printf("Numero removido\n");
							}
							else{
								printf("Numero nao encontrado\n");
							}
							break;
						case 4:
							head = removeHead(head);
						default:
							break;
					}
				}
				esc=3;
				break;
			case 4:
				while(esc!=3){
					printf("Qual operacao deseja\n");
					printf("(1) Inserir\n(2) Remover\n(3) Voltar\n");	
					scanf("%d",&esc);
					switch(esc){
						case 1:
							printf("Qual numero deseja Inserir?: ");
							scanf("%d",&num);
							p = addStack(p,num);
							break;
						case 2:
							p = removeStack(p);
							break;
						default:
							break;

					}

				}
				esc=4;
				break;
			case 5:
				while(esc!=3){
					printf("Qual operacao deseja\n");
					printf("(1) Inserir\n(2) Remover\n(3) Voltar\n");	
					scanf("%d",&esc);
					switch(esc){
						case 1:
							printf("Qual numero deseja Inserir?: ");
							scanf("%d",&num);
							f = addQueue(f,num);
							break;
						case 2:
							f = removeQueue(f);
							break;
						default:
							break;

					}
				}
				esc=5;
				break;
			default:
				break;

		}
	}

	// desaloca todas as estruturas no final do programa
	if(h!=NULL){
		while(h->prox!=NULL){
			removeListSimOrd(h,h->prox->data);
		}
		free(h);
		h=NULL;
	}
	if(c!=NULL){
		while(c->prox!=NULL){
			removeListSimOrd(c,c->prox->data);
		}
		free(c);
		c=NULL;
	}
	if(head!=NULL){
		while(head->prox!=NULL){
			removeListDup(head,head->prox->data);
		}
		free(head);
		head=NULL;
	}
	while(f!=NULL){
		f = removeQueue(f);
	}
	while(p!=NULL){
		p = removeStack(p);
	}

	gfx_quit();
	
	return 1;

}
