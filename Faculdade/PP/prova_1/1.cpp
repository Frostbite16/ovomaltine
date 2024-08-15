#include <iostream>

using namespace std;

template<class T=int>
class no{
    
    T data;
    no<T>* prox;

    public:
        no(T data=0, no<T> *prox=nullptr): data(data), prox(prox)
        {} 
        
        T& getData(){
            return data;
        }           
        
        no<T>*& getProx(){
            return prox;
        }

        void setProx(no<T> *prox){
            no::prox = prox;
        }
    
};



template<class T=int>

class DFIFO{
    
    no<T> *ptr; 
    
    public:
        DFIFO(): ptr(nullptr)
        {} 
        void push(T val){
            no<T>** ptrProx;
            ptrProx = &ptr;
            
            while((*ptrProx)!=nullptr){
                ptrProx = &(*ptrProx)->getProx();
            } 
            *(ptrProx) = new no<T>(val);
        }
        T pop(){
            T ret;
            no<T> *ptAux;

            ret = ptr->getData();
            ptAux = ptr;
            
            delete ptAux;

            return ret;
             
        
        
        }
        T& operator[](unsigned index){
            unsigned cont;
            no<T> *ptAux;
            
            for(cont = 0, ptAux=ptr; cont!=index; ++cont, ptAux = ptAux->getProx()){}
            
            return ptAux->getData();

        }

        ~DFIFO(){
            no<T>* ptAux;
            while(ptr!=nullptr){
                ptAux = ptr;
                ptr = ptr->getProx();
                delete ptAux;
           
            }

        }

};

int main(){
    DFIFO<int> fila;
    int i;
    for(i=0;i<=10;++i){
       fila.push(i); 
       cout << fila[i] << endl;
    }    


}



