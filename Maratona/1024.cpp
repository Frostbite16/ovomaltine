#include <iostream>
#include <string>

using namespace std;


int main(){

    unsigned n,aux;
    string palavra;

    
    cin >> n;
    cin.ignore();
    for(int i=0; i<n; ++i){

        getline(cin,palavra);
        
        for(int j=0; j<palavra.size();j++){
            if((palavra[j]>=65)&&(palavra[j]<=90)||(palavra[j]>=97)&&(palavra[j]<=122)){
                palavra[j]+=3;
            }
        }
        
        for(int j=0; j<palavra.size()/2;j++){
            aux = palavra[j];
            palavra[j] = palavra[palavra.size()-j-1];
            palavra[palavra.size()-1-j] = aux;
        }
        
        for(int j=palavra.size()/2; j<palavra.size();j++){
            palavra[j]--;
        }
        cout << palavra;
    }
}