#include <iostream>
#include <cmath>
#include <ctime>

unsigned long long fatorial(int n) {
    unsigned long long f = 1;
    for (int i = 1; i <= n; ++i) {
        f *= i;
    }
    return f;
}

double cosTaylor(double x, int termos) {
    double coseno = 0.0;

    for (int n = 0; n < termos; ++n) {
        double termo = pow(-1, n) * pow(x, 2 * n) / fatorial(2 * n);
        coseno += termo;
    }

    return coseno;
}

int main() {
    double x;
    int termos;

    std::cout << "Digite o valor de x (em radianos): ";
    std::cin >> x;

    std::cout << "Digite o número de termos da série de Taylor: ";
    std::cin >> termos;
	
	clock_t begin = clock();
    double resultado = cosTaylor(x, termos);
	clock_t end = clock();
    std::cout << "cos(" << x << ") = " << resultado << std::endl;
	std::cout << "O algoritmo levou " << (double)(end-begin)/CLOCKS_PER_SEC << " segundos para ser calculado" << std::endl;

    return 0;
}

