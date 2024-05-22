// Codigo do osvaldo

#include <stdio.h>
#include <string.h>

#ifdef __unix__
    #include <unistd.h>
    #include <stdlib.h>

#elif defined(_WIN32) || defined(WIN32)

   #define OS_Windows

   #include <windows.h>

#endif


#define TRAVOU 'x'

void gets(char *,int, int *);
void split (char *, char *, char, int *);
int pertence (char, char *);
char delta(char, char);

void gets(char *str, int t, int *tamW){
     char c;	
     c=fgets(str,t,stdin);  //coloca \n e \0 no fim stdin é entrada padrao (teclado)
     *tamW=strlen(str)-1;
     str[*tamW]='\0';
}

//Separa cada elemento de tamanho 1 caracter de acordo com o separador.
//Para separar strings é necessário implementacao
void split (char *Q, char *str, char sep, int *tam)
{
    //SUGESTÃO: PODERIA strtok com um laço
    int i,j=0;
    for(i=0;i<*tam;i++)
        if (str[i]!=sep) //se não encontrou o separador
            Q[j++]=str[i];
    *tam = j; //guarda o tamanho dos elementos

}

//verifica se c pertence a Str e retorna a posição
int pertence (char c, char *Str){
    int i;

    for(i=0; Str[i]!='\0'; i++)
        if(c==Str[i])
            return i;

    return -1;  //retorna uma falha

}

char delta(char q, char c){ //implementar

}


int main()
{
    char W[20], Q[10], A[10], qo, F[10], q, mDelta[10][10];
    char str[30];
    int taux,tamQ, tamF, tamA, tamW, i, j, p=0;
    int Falha = 0;
    printf("Entre com o conjunto de Estados, separados por vígulas: ");
    gets(str,30,&tamQ);
    //Q=str.split(',') em Python
    split(Q, str, ',', &tamQ); //Separa cada estado
    
    printf("\nEntre os símbolos do alfabeto, separados por vígulas: ");
    gets(str,30,&tamA);
    split(A,str, ',', &tamA);
    printf("Entre com o estado inicial: ");
    //gets(str,2,&taux);
    //scanf(" %c%*[^\n]",&qo);
    qo=getchar(); 
    setbuf(stdin,NULL);  
    //getchar(); 
    //qo=str[0];    
    printf("\nEntre os estados de F, separados por vígulas: "); // implementar para que não se entre com símbolos que não estejam em Q
    gets(str,30,&tamF);
    split(F,str,',',&tamF);

    //Leitura da matriz Delta
    for (i=0; i<tamQ; i++)
        for(j=0;j<tamA;j++){
            printf("\ndelta(%c, %c) = ",Q[i],A[j]);
            scanf(" %c%*[^\n]",&mDelta[i][j]);//espaço em " %c%*[^n]" faz a leitura do buffer com '\n' e ignora
            //getchar();
           
            //fflush(stdin);  // ver o equivalente para ignorar o enter
            //__fpurge(stdin);
            //setbuf(stdin,NULL);
	    }
    getchar();
    printf("\nEntre com a palavra a ser verificada: ");
    gets(W,30,&tamW);
    printf("\nPalavra : %s\n",W);
//    q=qo;
//    c=W[p++]; //nextchar()
//    printf("\nSequencia de estados: ");
//    printf("%c ", q);
//    while (c!='\0') {
//        q=delta(q,c);  //implementar a função delta
//        if Travou(q){  //retornou -1 (exemplo)
//            Falha = 1;
//            break;}
//        c=W[p++]; //nextchar()
//        printf(",%c",q)} //c = ProximoChar
//    
//    if (!pertence(q,F) || Falha)      
//        printf("\nPalavra não reconhecida ");
//    else printf("Palavra reconhecida ");

}



