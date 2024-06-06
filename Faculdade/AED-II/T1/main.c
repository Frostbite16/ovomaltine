#include "gfx/gfx.h" /* gfx_rectangle(), gfx_ellipse(), gfx_innit(), gfx_quit(), gfx_paint*/
#include <unistd.h> /*sleep()*/
#include <math.h> /*sen(), MATH_PI, floor()*/
 
int drawnCsquare(unsigned x, unsigned y, unsigned c1, unsigned c2, unsigned times, unsigned d1, unsigned d2){//x,y tamanho dos lados, c1,c2 = ponto central, times = condicao de parada da recursao, d1,d2 = direcao  
	int x1 = c1-x, y1= c2-y, x2= c1+x, y2=c2+y;

	//Caso o numero de recursoes ultrapasse 10
	if(times<11){
		gfx_rectangle(floor(x1),floor(y1),floor(x2),floor(y2));
	}
	else{
		return 0;
	}

	if(times>=1){
		// Usa soma vetorial para escontrar o pontos necessarios
		// Caso a um vetor com tamanho a, volte o quadrado, o programa nao executara a funcao desse lado
		if(c2+d2!=d1){
			drawnCsquare(x/2,y/2,c1,y2+y/2,times-1,y2,y/2);
		}
		if(c1+d2!=d1){	
			drawnCsquare(x/2,y/2,x2+x/2,c2,times-1,x2,x/2);
		}
		if(c2-d1!=d2){
			drawnCsquare(x/2,y/2,c1,y1-y/2,times-1,y1,y/2);
		}
		if(c1-d1!=d2){
			drawnCsquare(x/2,y/2,x1-x/2,c2,times-1,x1,x/2);
		}
	
	}
	return 1;

}

int drawncircle(unsigned x,unsigned y,unsigned r, unsigned times){ // x,y = Posicao central, r = raio, times = condição de parada da recursao
	
	float sen45 = sin(M_PI/4); //Seno de 45 graus

	if(times <= 11){ 
		gfx_ellipse(x,y,r,r); // Desenha a circunferencia
	}
	else{
		return 0;
	}

	if(times>=1){
		//cria um circulo entre o ponto central e o ponto diagonal com metade do raio 
		drawncircle((x+(x-r*sen45))/2,(y+(y-r*sen45))/2,r/2,times-1);
		drawncircle((x+(x-r*sen45))/2,(y+(y+r*sen45))/2,r/2,times-1);
		drawncircle((x+(x+r*sen45))/2,(y+(y+r*sen45))/2,r/2,times-1);
		drawncircle((x+(x+r*sen45))/2,(y+(y-r*sen45))/2,r/2,times-1);
	}
	return 1;
}


int main(){
	unsigned nivel,esc;

	printf("\nQuadrado(1), Circulo(2) ");
	scanf("%u",&esc);
	if((esc==1)||(esc==2)){//caso a escolha do usuario for 1 ou 2
		printf("\nNivel de recursao: "); // nivel de recursao do desenho
 		scanf("%u",&nivel);
		gfx_init(600,600,"a"); // Inicia a tela com tamanho 600x600

		//escolha do usuario
		switch(esc){
		
			case 1:
				drawnCsquare(100,100,300,300,nivel,0,0);
				break;

			case 2:
				drawncircle(300,300,250,nivel);
				break;

			default:
				break;
	
		
		}	

		gfx_paint(); // Pinta
		sleep(5); // Espera 5 segundos
		gfx_quit(); // Fecha a tela
	}
	return 0;


}



