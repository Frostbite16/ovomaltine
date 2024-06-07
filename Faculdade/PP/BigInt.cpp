#include <vector>
#include <iostream>
#include <cmath>

using namespace std;


class BigInt{
    vector<unsigned char> list;
    vector<unsigned char> aux;
    int n;
    public:
        BigInt(int n=0)
        {
            list.clear();
            aux.clear();
            this->n=n;
        }
       
        /*void pushFront(){
            list.resize(sizeof(unsigned char)* list.size()+1);

        }*/
       
        void convertFirst(){
            int num = n;
            unsigned char element;
            while(num>255){
                element = num%256;
                num = (num-element)/256;
                list.push_back(element);
            }    
            list.push_back(num);
            copyList();
        }

        int getNum() const{
            return n;
        }

        void copyList(){
            while(list.size()>aux.size()){
                aux.push_back(0);
            }
            for(int i=0;i<list.size();i++){
                aux[i]=list[i];
            }
        }

        void printList(){
            int num=0;
            for(int i=0; i<list.size();i++){
                cout << (int)list[i] << " ";       
            }
            
            
        }

        void convertAndAdd(int num){
            int ci=0, co=0, listSize = list.size();
            char element;
            while(num > 1){
                for(int i=0;i<listSize;i++){
                    if(i<=aux.size()){
                        if((list[i] + aux[i] + co)>255){
                            ci=list[i] + aux[i] + co;
                            ci = ci % 256;
                            list[i] = ci;
                            co = 1;
                        }
                        else{
                            list[i] += aux[i] + co;
                            co=0;
                        }
                        if((co==1)&&(i==listSize-1)){
                            list.push_back(co);
                            co=0;
                        }
                    }
                }
                num--;
                listSize = list.size();
            }
            copyList();
        }
};




int main(){
    int k;
    cin >> k;
    BigInt num(k);
    int numer = num.getNum() - 1;
    num.convertFirst();
    for(int i=numer; i>1; i--){
        num.convertAndAdd(i);
    }
    cout << "feito" << endl;
    num.printList();

}