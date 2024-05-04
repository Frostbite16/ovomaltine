#include <cstdio>
#include <iostream>

using namespace std;

int main(){

    int i,j,k;
    int n,m;
    int a,b,p;
    int grafo[100][100];
    float pm=1;
    cin >> n >> m;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            grafo[i][j] = 0;
        }
    }

    for(i=0;i<m;i++){
        cin >> a >> b >> p;
        grafo[a][b] = p;
        grafo[b][a] = a;
    }

    for(i=0;i<n;i++){
        grafo[i][i] = 0;
    }


    /*for(k=1;k<n;k++){
        for(i=1;i<n-1;i++){
            for(j=1;j<n;j++){
                if(grafo[i][j] > grafo[i][k] + grafo[k][j]){
                    grafo[i][j] = grafo[i][k] + grafo[k][j];
                }
            }
        }
    }*/

    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            printf("%d ", grafo[i][j]);
        }
        printf("\n ");
    }

    



}
