#include <cstdio>
#include <iostream>

using namespace std;

int main(){

    int i,j,k;
    int maior = 0;
    int im, jm;
    int n,m;
    int a,b,p;
    int grafo[100][100];
    int unv[100];
    int dist[100];
    float pm=1;
    cin >> n >> m;
    for(i=0;i<=n;i++){
        for(j=0;j<n;j++){
            grafo[i][j] = -1;
            
        }
    }

    for(i=0;i<m;i++){
        cin >> a >> b >> p;
        grafo[a][b] = p;
        grafo[b][a] = p;
    }

    for(i=0;i<n;i++){
        grafo[i][i] = 0;
        unv[i] = i;
    }

    for(i=1;i<=n;i++){
        dist[i] = grafo[1][i];
    }


    /*for(k=1;k<n;k++){
        for(i=1;i<n;i++){
            for(j=1;j<n;j++){
                if(grafo[i][j] < grafo[i][k] * grafo[k][j]){
                    grafo[i][j] = grafo[i][k] * grafo[k][j];
                }
            }
        }
    }*/
    j=1;
    
    
    for(i=0;i<=n;i++){
        for(j=0;j<=n;j++){
            printf("%d ", grafo[i][j]);
        }
        printf("\n ");
    }

    



}
