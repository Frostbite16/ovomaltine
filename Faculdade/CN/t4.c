#include<time.h>
#include<math.h>
#include<stdlib.h>
#include<stdio.h>

#define STEP 0.5f;


void initializeArray(double** constantList, size_t n){
	*constantList = (double*)malloc(sizeof(double)*(n+1));
} 

double polinomialFunction(size_t n, double x, double* constantList){
	double resultOverTime;
	for(size_t i=0; i<=n; ++i){
		resultOverTime += constantList[i] * pow(x,i);
	}
	return resultOverTime;	
}

void optInterval(double a, double b, size_t n, double* constantList){
	short unsigned flag=1;
	float aValue, bValue;

	bValue = polinomialFunction(n, b, constantList);

	while(flag){
		aValue = polinomialFunction(n, a+STEP, constantList);
		if(aValue*bValue<0){
			a+=STEP;
		}
		else{
			flag=0;
		}
	}	
	
	aValue = polinomialFunction(n, a, constantList);
	flag = 1;

	while(flag){
		if(aValue*bValue<0){
			b-=STEP;
		}
		else{
			flag=0;
		}
		bValue = polinomialFunction(n, b+STEP, constantList); 
	}

}

int main(){
	double *constantList, a, b, aValue, bValue;
	size_t ordem;

	printf("Qual a ordem da função: ");
	scanf("%ld", &ordem);

	initializeArray(&constantList, ordem);
	
	for(size_t i=0; i<=ordem; ++i){
		
		printf("Constante de X%ld: ",i);
		scanf("%lf", &constantList[i]);

	}
	

	do{
		
		printf("Intervalo [a;b]: ");
		scanf("%lf %lf",&a,&b);
		
		aValue = polinomialFunction(ordem, a, constantList);
		bValue = polinomialFunction(ordem, b, constantList);

	}while((aValue*bValue)>0);
	

	printf("Intervalo [ %lf ; %lf] \n", a,b);



	free(constantList);
	return 1;
}
