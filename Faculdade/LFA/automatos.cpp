
#include <iostream> /* cin, cout */
#include <string> /* string */
#define size 10

using namespace std;

class automato{
    char estados[size], estadosFinais[size], alfabeto[size], est_Inicial;
    string PalavraAtual[30], palavraAceita[30]; 
    char matrizAlpha[size][size];
    int estadoSize=0, alfaSize=0;
    
    public:
        
        bool addEstados(char c){ // Adiciona novos estados com nome c na posicao i do vetor estados[] caso o estado não exista
            for(int i=0; i<estadoSize; ++i){
                if(c==estados[i]){
                    cout << "Estado já existe";
                    return false;
                }
            }
            estadoSize+=1;
            estados[estadoSize-1]=c;
            return true;
        }

        void addEstadosFinais(char c, int i){ // Adiciona novos estados finais com nome c na posicao i do vetor estadosFinais[]
            if(checkPar(c,estados)){
                estadosFinais[i] = c;
            }
            else{
                cout << "Estado não encontrado";
            }
        }

        bool addAlfabeto(char c, int i){ // Adiciona novos simbolos c na posicao i do vetor alfabeto[]
            for(int i=0; i<alfaSize; ++i){
                if(c==alfabeto[i]){
                    cout << "Simbolo já está dentro do alfabeto";
                    return false;
                }
            }
            alfaSize++;
            estados[alfaSize-1]=c;
            return true;
        }
        
        bool checkPar(char c, char *Par){ // Checa se um simbolo já está na em um vetor Par, caso esteja retorna falso
            for(int i=0;i<size;++i){
                if(c==Par[i]){
                    return true;
                }
            }
            return false;
        }

        void funcTransicao(unsigned i, unsigned j, unsigned c){ // adiciona novas relacoes entre os estados
            

        }

    







    public:


};
