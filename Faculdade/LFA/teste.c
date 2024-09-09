#include<stdio.h>
typedef struct struc{
	char* something;
	unsigned size;	
}struc;

int main(){
	char *matrix[2][2];
	/*for(int i=0; i<=1; i++){
		for(int j=0; j<=1; j++){
			matrix[i][j] = malloc(sizeof(struc));
			puts("a");
		}
	}*/
	matrix[0][0] = "teste";
	matrix[0][1] = "teste2";
	matrix[1][0] = "teste3";

	
	printf("%s, %s, %s", matrix[0][0], matrix[0][1], matrix[1][0]);

}
