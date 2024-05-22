
#include <iostream> /* cin, cout */
#include <string> /* string */

using namespace std;

class automato{
    char estados[10], estadosFinais[10], alfabeto[10], est_Inicial;
    string PalavraAtual[30], palavraAceita[30]; 
    char matrizAlpha[10][10];
    int alphaSize, clock;
    
    public:
        
        void addEstados(char c, int i){ // Adiciona novos estados com nome c na posicao i do vetor estados[]

        }

        void addEstadosFinais(char c, int i){ // Adiciona novos estados finais com nome c na posicao i do vetor estadosFinais[]
            estadosFinais[i] = c;
        }

        void addAlfabeto(char c, int i){ // Adiciona novos simbolos c na posicao i do vetor alfabeto[]
            alfabeto[i] = c;
        }
        
        bool checkAlfa(char c){ // Checa se um simbolo já está na no vetor alfabeto[], caso esteja retorna falso
            for(int i=0;i<alphaSize;++i){
                if(c==alfabeto[i]){
                    return true;
                }
            }
            return false;
        }

        void funcTransicao(unsigned i, unsigned j, unsigned c){ // adiciona novas relacoes entre os estados
            

        }

    







    public:


};
