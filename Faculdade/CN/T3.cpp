#include <iostream>
#include <iomanip>
#include <cmath>

#define MAX_ITERACOES 1000  // Limite de iterações para evitar loops infinitos

// Função para calcular o erro absoluto
double calcularErro(double novo, double anterior) {
    return fabs(novo - anterior);
}

// Função que executa o método de Gauss-Seidel
void gaussSeidel(int n, double a[10][10], double b[10], double x[10], double precisao) {
    double novoX[10], erro[10], erroTotal;
    int iteracao = 0;
    
    std::cout << std::fixed << std::setprecision(7);
    
    while (true) {
        iteracao++;
        bool convergencia = true;
		erroTotal = 0.0;

        // Exibindo a iteração atual
        std::cout << "Iteração " << iteracao << ":\n";
        
        for (int i = 0; i < n; i++) {
            novoX[i] = b[i];
            
            for (int j = 0; j < n; j++) {
                if (i != j) {
                    novoX[i] -= a[i][j] * x[j];
                }
            }
            
            novoX[i] /= a[i][i];

            // Calculando o erro absoluto para cada variável
            erro[i] = calcularErro(novoX[i], x[i]);

            // Atualiza o erro total com o maior erro entre as variáveis (norma L∞)
            if (erro[i] > erroTotal) {
                erroTotal = erro[i];
            }

            if (erro[i] > precisao) {
                convergencia = false;
            }
            
            x[i] = novoX[i];
        }
        
        // Exibindo o estado atual das soluções
        std::cout << "Soluções atuais: ";
        for (int i = 0; i < n; i++) {
            std::cout << "x[" << i << "] = " << x[i] << " ";
        }
        std::cout << "\n";

        // Exibindo os erros atuais
        /*std::cout << "Erros atuais: ";
        for (int i = 0; i < n; i++) {
            std::cout << "Erro[" << i << "] = " << erro[i] << " ";
        }
        std::cout << "\n\n";*/

		 // Exibindo o erro total da iteração
        std::cout << "Erro total da iteração: " << erroTotal << "\n\n";

        // Critério de parada
        if (convergencia || iteracao >= MAX_ITERACOES) {
            break;
        }
    }
    
    std::cout << "Solução final após " << iteracao << " iterações:\n";
    for (int i = 0; i < n; i++) {
        std::cout << "x[" << i << "] = " << x[i] << "\n";
    }
}

int main() {
    int n;
    double precisao;
    double a[10][10], b[10], x[10];

    std::cout << "Digite a ordem da matriz: ";
    std::cin >> n;

    std::cout << "Digite os coeficientes da matriz (linha por linha):\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cin >> a[i][j];
        }
    }

    std::cout << "Digite os termos independentes (vetor b):\n";
    for (int i = 0; i < n; i++) {
        std::cin >> b[i];
    }

    std::cout << "Digite a solução inicial:\n";
    for (int i = 0; i < n; i++) {
        std::cin >> x[i];
    }

    std::cout << "Digite a precisão desejada: ";
    std::cin >> precisao;
	clock_t begin = clock();
    gaussSeidel(n, a, b, x, precisao);
	clock_t end = clock();
	std::cout << "O algoritmo levou " << (double)(end-begin)/CLOCKS_PER_SEC << " segundos para ser calculado" << std::endl;

    return 0;
}
