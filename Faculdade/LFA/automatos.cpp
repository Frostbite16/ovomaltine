
#include <iostream> /* cin, cout */
#include <string> /* string */
#include <cstdio> /* getchar() */


using namespace std;

class automato{
    string estados, estadosFinais, alfabeto;
    string PalavraAtual, palavraAceita; 
    char matrizAlpha[10][10],estInicial;
    int estadoSize=0, alfaSize=0, clock=0;
    int i,j,k;
    
    public:
        
        automato()
        {}
        
        void addEstados(){ // Adiciona novos estados com nome c na posicao i do vetor estados[] caso o estado não exista
            getline(cin, estados);

            for(i=0;i<estados.size();i++){
                if(estados[i]==','){

                    clock++;
                    for(j=i;j<estados.size();j++){
                        estados[j]=estados[j+1];

                    }
                }
            }

            estados.resize(estados.size()-clock);
            clock=0;
        }
        
        int CheckEstados(char c){
            for(i=0;i<estados.size();i++){
                if(c==estados[i])
                    return i;
            }
            return NULL;
        }
        
        bool setEstInicial(){
            char c = getchar();
            if(CheckEstados(c)!=NULL){
                estInicial = c;
                return true;
            }
            return false;
            
        }

        void addEstadosFinais(){ // Adiciona novos estados finais com nome c na posicao i do vetor estadosFinais[]
            getline(cin, estadosFinais);

            for(i=0;i<estadosFinais.size();i++){

                if(estadosFinais[i]==','){

                    clock++;
                    for(j=i;j<estadosFinais.size();j++){
                        estadosFinais[j]=estadosFinais[j+1];
                    }
                }
            }
            estadosFinais.resize(estadosFinais.size()-clock);
            clock=0;
            for(i=0;i<estadosFinais.size();i++){
                if(CheckEstados(estadosFinais)==NULL){
                    clock++;
                    if(!(i==estadosFinais.size()-1)){
                        for(j=i;j<estadosFinais.size()-1;j++){
                            estadosFinais[j]=estadosFinais[j+1];
                        }
                    }
                    estadosFinais.resize(estadosFinais.size()-clock);
                    clock=0;
     
                }
            }
            
        }

        void addAlfabeto(){ // Adiciona novos simbolos c na posicao i do vetor alfabeto[]
            getline(cin, alfabeto);
            for(i=0;i<alfabeto.size();i++){

                if(alfabeto[i]==','){

                    clock++;
                    for(j=i;j<alfabeto.size();j++){

                        alfabeto[j]=alfabeto[j+1];
                    }
                }
            }
            alfabeto.resize(alfabeto.size()-clock);
            clock=0;
        }
        
        bool checkPar(char c, string *Par){ // Checa se um simbolo já está na em um vetor Par, caso esteja retorna falso
            for(int i=0;i<Par->size();i++){
                if(c==Par[i]){
                    return true;
                }
            }
            return false;
        }

        void funcTransicao(){ // adiciona novas relacoes entre os estados
            for(i=0;i<estados.size();i++){
                for(j=0;j<alfabeto.size();j++){
                    printf("Transicao(%c, %c)\n",estados[i],alfabeto[j]);
                    scanf("%c",&matrizAlpha[i][j]);
                }
            }

        }

        void rodarAutomato(){
            bool travou=false;
            char proxEst='', atualEst=estInicial,inputChar;
            string input;
            i = CheckEstados(estInicial);
            while((proxEst=='\0')&&!(checkPar(atualEst, estadosFinais)){
                inputChar=getchar();
                if(inputChar==''){
                     proxEst='\0';

}
                else if(checkPar(inputChar, alfabeto){
                     travou=1;
                }
                else{
                     
                }

            }
        }


};

int main(){
    automato a1;
    a1.addAlfabeto();
    a1.addEstados();
    a1.addEstadosFinais();
    a1.funcTransicao();
}