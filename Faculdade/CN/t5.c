#include <stdio.h>
#include <math.h>
#include <time.h>

// Função para calcular a raiz de ordem m de p
double f(double x, double p, int m) {
    return pow(x, m) - p;
}



int main() {
    double p, x, previous_x;
    int m, max_iter = 1000, i;
    clock_t start, end;
    double time_taken;

    // Entrada do valor p e da ordem da raiz m
    printf("Digite o número p (para calcular a raiz): ");
    scanf("%lf", &p);
    printf("Digite a ordem da raiz m: ");
    scanf("%d", &m);

    // Chute inicial para o valor de x
    x = 1;

    // Iniciar o cálculo do tempo
    start = clock();

    for (i = 0; i < max_iter; i++) {
        double fx = f(x, p, m);
			
        printf("Iteração %d: x = %.7lf, f(x) = %.7lf\n", i, x, fx);

		previous_x = x;
        x = (1.0/m)*((m-1)*x + p/pow(x,m-1));

        if (fabs(x-previous_x) < 1e-7) { // Considerando precisão nula (assumindo como um número muito pequeno)
            printf("Raiz de ordem %d encontrada para %.7lf: %.7lf\n", m, p, x);
            printf("Número de iterações: %d\n", i);
            break;
        }

				
    }

    if (i == max_iter) {
        printf("Número máximo de iterações atingido.\n");
    }

    // Cálculo do tempo de execução
    end = clock();
    time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Tempo de execução: %lf segundos\n", time_taken);

    return 0;
}

