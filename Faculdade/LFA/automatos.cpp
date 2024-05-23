
#include <iostream> /* cin, cout */
#include <string> /* string */
#include <cstdio> /* getchar() */


using namespace std;

class automato{
    string estados, estadosFinais, alfabeto;
    string PalavraAtual, palavraAceita; 
    char matrizDelta[10][10],estInicial;
    int estadoSize=0, alfaSize=0, clock=0;
 
    
    public:
        
        automato()
        {}
        
        void addEstados(){ // Adiciona novos estados com nome c na posicao i do vetor estados[] caso o estado não exista
            cin >> ws;
            getline(cin, estados);

            for(unsigned i=0;i<estados.size();i++){
                if(estados[i]==','){

                    clock++;
                    for(unsigned j=i;j<estados.size();j++){
                        estados[j]=estados[j+1];

                    }
                }
            }

            estados.resize(estados.size()-clock);
            clock=0;
        }
        
        int CheckEstados(char c){
            for(unsigned i=0;i<estados.size();i++){
                if(c==estados[i])
                    return i;
            }
            return -1;
        }
        
        bool setEstInicial(){
            char c = getchar();
            while((getchar()) != '\n');
            if(CheckEstados(c)!=-1){
                estInicial = c;
                return true;
            }
            return false;
            
        }

        void addEstadosFinais(){ // Adiciona novos estados finais com nome c na posicao i do vetor estadosFinais[]
            cin >> ws;
            getline(cin, estadosFinais);

            for(unsigned i=0;i<estadosFinais.size();i++){

                if(estadosFinais[i]==','){

                    clock++;
                    for(unsigned j=i;j<estadosFinais.size();j++){
                        estadosFinais[j]=estadosFinais[j+1];
                    }
                }
            }
            estadosFinais.resize(estadosFinais.size()-clock);
            clock=0;

            for(unsigned i=0;i<estadosFinais.size();i++){
                if(CheckEstados(estadosFinais[i])==-1){
                    clock++;
                    if(!(i==estadosFinais.size()-1)){
                        for(unsigned j=i;j<estadosFinais.size()-1;j++){
                            estadosFinais[j]=estadosFinais[j+1];
                        }
                    }
                    estadosFinais.resize(estadosFinais.size()-clock);
                    clock=0;
     
                }
            }
            
        }

        void addAlfabeto(){ // Adiciona novos simbolos c na posicao i do vetor alfabeto[]
            cin >> ws;
            getline(cin, alfabeto);
            for(unsigned i=0;i<alfabeto.size();i++){

                if(alfabeto[i]==','){

                    clock++;
                    for(unsigned j=i;j<alfabeto.size();j++){

                        alfabeto[j]=alfabeto[j+1];
                    }
                }
            }
            alfabeto.resize(alfabeto.size()-clock);
            clock=0;
        }
        
        bool checkEstFinais(char c){ // Checa se um simbolo já está no vetor estadosFinais, caso esteja retorna verdadeiro
            for(unsigned i=0;i<estadosFinais.size();i++){
                if(c==estadosFinais[i]){
                    return true;
                }
            }
            return false;
        }

        int checkAlfabeto(char c){ // Checa se um simbolo já está no vetor alfabeto, caso esteja retorna o indice
            for(unsigned i=0;i<alfabeto.size();i++){
                if(c==alfabeto[i]){
                    return i;
                }
            }
            return -1;
        }

        void inicializarMatrix(){
            for(unsigned i=0;i<estados.size();i++){
                for(unsigned j=0;j<alfabeto.size();j++){
                    matrizDelta[i][j] = ' '; 
                }
            }
        }

        void funcTransicao(){ // adiciona novas relacoes entre os estados
            for(unsigned i=0;i<estados.size();i++){
                for(unsigned j=0;j<alfabeto.size();j++){
                    printf("Transicao(%c, %c)\n",estados[i],alfabeto[j]);
                    matrizDelta[i][j] = getchar();
                    if(CheckEstados(matrizDelta[i][j])==-1){
                        matrizDelta[i][j] = '-';
                    }
                    while((getchar()) != '\n');
                    
                    
                }
            }

        }

        void rodarAutomato(){
            bool travou=false;
            char proxEst=' ', atualEst=estInicial,inputChar;
            string input;
            int i = CheckEstados(estInicial),j;
            printf("Estado atual: %c \n", estados[i]);
            while((proxEst!='\0')||!(checkEstFinais(atualEst))){
                printf("Aguardando entrada do usuario...\n");
                inputChar=getchar();
                while((getchar()) != '\n');
                j=checkAlfabeto(inputChar);
                if(j==-1){
                    travou=1;
                    
                }
                else{
                    
                    proxEst = matrizDelta[i][j];
                    
                    if(proxEst=='-'){
                        travou=1;
                    }
                }

                if((travou==0)&&(inputChar!='-')){
                    atualEst=proxEst;
                    i = CheckEstados(atualEst);
                }
                else if(inputChar!='-'){
                    printf("Falha na mudanca de estado!! input rejeitado\n");
                    travou=0;
                }
                
                printf("Estado atual: %c \n", estados[i]);

                if(inputChar=='-'){
                    proxEst='\0';
                    travou = 0;

                }
                
                if(checkEstFinais(atualEst)){
                    cout << "Estado final Selecionado digite '-' para terminar" << endl;
                }     
                

            }
        }


};

int main(){
    automato a1;
    int continuar = 1;

    cout << "Adicionar alfabeto, separados por virgula" << endl;
    a1.addAlfabeto();
    cout << "Adicionar estados, separados por virgula" << endl;
    a1.addEstados();
    cout << "Qual e o estado inicial? " << endl;
    while(!(a1.setEstInicial())){
        cout << "Estado nao encontrado, digite novamente" << endl;
    }
    cout << "Adicionar estados finais, separados por virgula" << endl;
    a1.addEstadosFinais();
    a1.inicializarMatrix();
    cout << "Descreva a funcao de transicao" << endl;
    a1.funcTransicao();
    while(continuar==1){
        a1.rodarAutomato();
        cout << "Deseja rodar novamente outra palavra? 1 para sim e 0 para nao " << endl;
        cin >> continuar;
    }
        
    
}
