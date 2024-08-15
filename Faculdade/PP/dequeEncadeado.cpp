#include <iostream>

using namespace std;

template<class T=int>

class node{
    T data;
    node* prox;
    public: 
        
        node(){
            this->data = NULL;
            this->prox =  NULL;
        }

        node(T data){
            this->data = data;
            this->prox =  NULL;
        }

};

template<class T=int>
class deque{
    node<> *firstElement, *LastElement;

    public:
        deque(){
            firstElement=NULL, LastElement=NULL;
        }

};