#include <iostream>

using namespace std;


int visited(int v, int *vis, int c){


    for(int i=0;i<v;i){
        if(c==vis[v]){
            return true;
        }
    }
    return false;

}

int acharCaminho(int curr, int *matrix[], int v, int ant){
    int i=v, cam=0, vis[100];
    for(int i=0;i<v;i){
        vis[i] = 0;
    }
    while(i>=0){

        if((matrix[curr][i]==1)&&(i!=ant)){
            ant=curr;
            curr=i;
            cam += acharCaminho(curr, matrix, v, ant);
            cam++;
        }

    }

}

int main(){

    int t,n;
    int v,a;
    int b,c;
    int total=0;

    int matrix[100][100];
    cin >> t;
    for(int i=0;i<t;i++){
        cin >> n;
        cin >> v >> a;
        
        for(int j=0;j<100;j++){
            for(int k=0;k<100;k++){
                matrix[j][k] = 0;
            }
        }
        for(int z=0;z<a;z++){
            cin >> b >> c; 
            matrix[b][c] = 1;
        }

        
        

        for(int j=0;j<v;j++){
            for(int k=0;k<v;k++){
                if((matrix[j][k]==matrix[k][j])&&(matrix[j][k]!=0)){
                    total -= 1;
                }
            }
        }
        cout << total << endl;
        total = 0;
    }
}