#include <stdio.h>
#include <math.h>
#include <time.h>

#define N 10  // Defina o tamanho da matriz (3x3 neste caso)
			  

void imprimeMatriz(double mat[N][N], double b[N], size_t n) {
    printf("Estado atual da matriz:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%10.4lf ", mat[i][j]);
        }
        printf("| %10.4lf\n", b[i]);
    }
    printf("\n");
}

void trocaLinhas(double mat[N][N], double b[N], int linha1, int linha2, unsigned n) {
    double temp;
    for (int i = 0; i < n; i++) {
        temp = mat[linha1][i];
        mat[linha1][i] = mat[linha2][i];
        mat[linha2][i] = temp;
    }
    temp = b[linha1];
    b[linha1] = b[linha2];
    b[linha2] = temp;
	printf("Após trocar a linha %d com a linha %d:\n", linha1 + 1, linha2 + 1);
    imprimeMatriz(mat, b, n);
}

double eliminacaoGauss(double mat[N][N], double b[N], size_t n) {
    int trocas = 0; // Contador de trocas de linha para o cálculo do sinal
    double det = 1.0;

    for (int k = 0; k < n - 1; k++) {
        // Pivotação Parcial
        int iMax = k;
        for (int i = k + 1; i < n; i++) {
            if (fabs(mat[i][k]) > fabs(mat[iMax][k])) {
                iMax = i;
            }
        }
        
		if (fabs(mat[iMax][k]) < 1e-9) {
            det = 0;
            return det; // Se o pivô é zero, a matriz é singular, e a determinante é zero
        }

        if (iMax != k) {
            trocaLinhas(mat, b, k, iMax,n);
            trocas++;
        }
		
		
        // Eliminação de Gauss
        for (int i = k + 1; i < N; i++) {
            double fator = mat[i][k] / mat[k][k];
            for (int j = k; j < N; j++) {
                mat[i][j] -= fator * mat[k][j];
            }
            b[i] -= fator * b[k];
       		printf("Após eliminar a linha %d usando a linha %d:\n", i + 1, k + 1);
        	imprimeMatriz(mat, b, n);
		}
	
    }

    for (int i = 0; i < n; i++) {
        det *= mat[i][i];
    }

    if (trocas % 2 != 0) {
        det = -det;
    }

    return det;
}

void substituicaoReversa(double mat[N][N], double b[N], double x[N], unsigned n) {
    for (int i = n - 1; i >= 0; i--) {
        x[i] = b[i];
        for (int j = i + 1; j < n; j++) {
            x[i] -= mat[i][j] * x[j];
        }
        x[i] /= mat[i][i];
    }
}

int main() {
    double mat[N][N];
    double Elem ,b[N], x[N], deter, tempo;	
	size_t escOrdem, n;
	
	printf("Qual é a norma da matriz? ");
	scanf("%lu", &n); 
	
	for(size_t i=0; i<n; i++){
		for(size_t j=0; j<n; j++){
			printf("Elemento de [%lu][%lu]: ",i,j);
			scanf("%lf",&mat[i][j]);
		}
		printf("Resposta da equação [%lu]: ",i);
		scanf("%lf",&b[i]);
	}
	
	clock_t begin = clock();

	printf("Matriz inicial:\n");
    imprimeMatriz(mat, b, n);

    deter = eliminacaoGauss(mat, b, n);
    substituicaoReversa(mat, b, x, n);
	clock_t end = clock();

	tempo = (double)(end-begin)/CLOCKS_PER_SEC;
    if (deter != 0) {
        substituicaoReversa(mat, b, x, n);

        printf("Soluções:\n");
        for (int i = 0; i < n; i++) {
            printf("x[%d] = %lf\n", i, x[i]);
        }
    } else {
        printf("A matriz é singular; o sistema não tem solução única.\n");
    }

	printf("A determinante é: %.lf\n", deter);
	printf("O tempo gasto foi de: %.7lf segundos\n", tempo);
	
    return 0;
}

