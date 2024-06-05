#include <cstdio>
#include <iostream>

using namespace std;

int main(){

    int n,m;
    int a,b,p;
    int i,j,k;
    double distancia[100][100];
    double grafo[100][100];
    int maior[100],ma;

    cin >> n >> m;
    
    for(i=0; i<=n; i++){
        for(j=0; j<=n; j++){
            distancia[i][j] = 100000;
            grafo[i][j] = 0;
        }
    }
    for(i=0; i<m; i++){
        cin >> a >> b >> p;
        distancia[a][b] = 100-p;
        distancia[b][a] = 100-p;
    }
    for(i=1; i<=n; i++){
        distancia[i][i] = 0;
    }
    for(k=1;k<=n;k++){
        for(i=1;i<=n;i++){
            for(j=1;j<=n;j++){
                if((distancia[i][j] > (distancia[i][k] + distancia[k][j]))&&(distancia[i][k]<100000)&&(distancia[k][j]<100000)){
                    distancia[i][j] = (distancia[i][k] + distancia[k][j]);
                }
                
            }
            
            
            
        }
        maior[k] = distancia[1][n];

        
        
    }

    for(i=1; i<=n; i++){
        for(j=1; j<=n; j++){
            /*if((distancia[i][j]!=0)&&(distancia[i][j]<100)){
               distancia[i][j] = 100-distancia[i][j];
            }*/
           //cout << distancia[i][j] << " ";
        }
        cout << maior[i] << " ";
       cout << endl;
    }

    



}
