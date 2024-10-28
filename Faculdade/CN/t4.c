#include<time.h>
#include<math.h>
#include<stdlib.h>
#include<stdio.h>

#define STEP 0.0001


void initializeArray(double** constantList, size_t n){
	*constantList = (double*)malloc(sizeof(double)*(n+1));
} 

double polinomialFunction(size_t n, double x, double* constantList){
	double resultOverTime=0;
	for(size_t i=0; i<=n; ++i){
		resultOverTime += constantList[i] * pow(x,i);
	}
	return resultOverTime;	
}

void findZero(double a, double b, size_t n, double* constantList){
	short unsigned flag;
	double bValue, aValue, bAux, firstB, firstBValue;
	bAux = b;
	firstB = b;

	bValue = polinomialFunction(n, b, constantList);
	aValue = polinomialFunction(n, a, constantList);

	firstBValue = bValue;
	do{
		bValue = firstBValue;
		while((aValue*bValue<0)){
			aValue = polinomialFunction(n, a+STEP, constantList);
			if(aValue*bValue<0){
				a += STEP;			
			}			
		}
	
		flag = 0;

		aValue = polinomialFunction(n, a, constantList);
		while(a<bAux){
			bValue = polinomialFunction(n, bAux-STEP, constantList);
			bAux-=STEP;
			if(bValue*aValue<0){
				b = bAux;
				flag=1;
			}
		}
		
		bValue = polinomialFunction(n, b, constantList);
		
		while((aValue*bValue<0)){
			aValue = polinomialFunction(n, a+STEP, constantList);
			if(aValue*bValue<0){
				a += STEP;			
			}			
		}
		aValue = polinomialFunction(n, a, constantList);

		if(aValue*bValue<0){
			printf("Raiz no intervalo [%.7lf,%.7lf]\n Valores de F(a): %lf, F(b): %lf\n",a,b,aValue,bValue); 
		}

		a = b;
		b = firstB;

	}while(flag);
}

void optInterval(double *a, double *b, size_t n, double* constantList){
	short unsigned flag=1;
	double aValue, bValue;

	bValue = polinomialFunction(n, *b, constantList);


}



int main(){
	double *constantList, a, b, aValue, bValue;
	size_t ordem;
	clock_t end,begin;

	printf("Qual a ordem da função: ");
	scanf("%ld", &ordem);

	initializeArray(&constantList, ordem);
	
	for(size_t i=0; i<=ordem; ++i){
		
		printf("Constante de X%ld: ",i);
		scanf("%lf", &constantList[i]);

	}
	

	printf("Intervalo [a;b]: ");
	scanf("%lf %lf",&a,&b);
	

    begin = clock();

	findZero(a, b, ordem, constantList);

	end = clock();

	printf("Tempo: %lf",(double)(end-begin)/(CLOCKS_PER_SEC));


	free(constantList);
	return 1;
}
