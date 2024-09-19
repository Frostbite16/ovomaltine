//
// Created by delair on 05/09/24.
//

#include "DAppCalcArea.h"
#include <iostream>
using namespace std;
void DAppCalcArea::registraGeometria(DGeometriaApp * pGeo) {
    registrosDeGeometrias.push_back(pGeo->alocarGeoApp());
}

void DAppCalcArea::mainDoApp() {
    unsigned numeroLido;
    vector<DGeometriaApp*>::iterator it;
    unsigned i;
    do{
        cout<<"Digite 0 (para sair)";
        for(it=registrosDeGeometrias.begin(),i=1;
            it!=registrosDeGeometrias.end();
            ++it,++i){
            cout<<","<<i<<"(para "<<(*it)->getNome()<<')';
        }
        cout<<':'<<endl;
        cin>>numeroLido;
        if(numeroLido!=0){
            registrosDeGeometrias.at(numeroLido-1)->lerParametrosDaGeometria();
            calc.insert((DGeometria*)registrosDeGeometrias.at(numeroLido-1));
        }
    }while(numeroLido!=0);
    cout<<endl<<"Area total:"<<calc.areTotal()<<endl;
}

DAppCalcArea::~DAppCalcArea() {
        vector<DGeometriaApp *>::iterator it;
        for (it = registrosDeGeometrias.begin(); it != registrosDeGeometrias.end(); ++it)
            delete *it;
}
