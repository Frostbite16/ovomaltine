#include <iostream>
#include <string>

using namespace std;

int main(){

    int n;
    int i,j,k;
    int lastEsqDim=-1;
    int quantDiam=0;
    int clock=0;
    int removeSize = 0;
    string diamantes,jpline;

    cin >> n;
    getline(cin,jpline);
    for(i=1;i<=n;++i){
        lastEsqDim=-1;
        quantDiam = 0;
        removeSize = 0;
        diamantes.clear();
        getline(cin, diamantes);
        
        for(j=0;j<diamantes.size();++j){

            if(diamantes[j]=='<'){
                lastEsqDim = j;
            }
            if((lastEsqDim!=-1)&&(diamantes[j]=='>')){
                quantDiam+=1;
                for(k=lastEsqDim;k<=j;++k){
                    diamantes[k] = '0';
                    removeSize += 1;
                }
                for(k=j+1;k<diamantes.size();++k){
                    diamantes[lastEsqDim+clock] = diamantes[k];
                    clock += 1;
                }
                clock = 0;
                j = -1;
                lastEsqDim=-1;
                diamantes.resize(diamantes.size()-removeSize);
                removeSize = 0;
            }
        }
        cout << quantDiam << endl;
    }



}