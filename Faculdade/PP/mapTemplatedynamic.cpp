//Implementar redimensionamente
//Array de chaves ordenadas
//Busca binaria Funcao finde(K, unsigned)

#include <iostream>
#include <cstdlib>

using namespace std;

template <class K, class V>

class myMap{
    K *chave;
    V *dados;
    unsigned size,index;

    unsigned findKey(K key, unsigned &index){
        int i;
        for(i=0;i<size;i++){
            if(chave[i]==key){
                index = i;
                return true;
            }
        }
        return false;
    }

    public:
        
        myMap(){
            chave = (K)(malloc(sizeof(K)));
            data = (V)(malloc(sizeof(V)));
            size = 0;
        }

        bool addElem(K key, V data){
            int i;
            size++;
            if(findKey()){
               return 0; 
            }    
            chave = (K)(realloc(sizeof(K)*size));
            dados = (V)(realloc(sizeof(V)*size));
            chave[size-1] = key;
            dados[size-1] = data;
            return 1; 
                
        }

        V at(T key){
            int index;
            if(findKey(key, index)){
                return data[index];
            }
            throw "Chave não encontrada"
        }

        bool remove()


};