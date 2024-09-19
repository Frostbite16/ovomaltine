//
// Created by delair on 05/09/24.
//

#include "DGeometriaApp.h"
#include <iostream>
using namespace std;
const string &DGeometriaApp::getNome() const {
    return nome;
}

void DGeometriaApp::setNome(const string &nome) {
    DGeometriaApp::nome = nome;
}

DQuadradoApp::DQuadradoApp(){
    DGeometriaApp::setNome("Quadrado");
}
void DQuadradoApp::lerParametrosDaGeometria() {
    float lado;
    cout<<endl<<"Digite lado:";
    cin>>lado;
    setLado(lado);
    cout<<endl;
}

DGeometriaApp *DQuadradoApp::alocarGeoApp() {
    return new DQuadradoApp;
}


DRetanguloApp::DRetanguloApp(){
    DGeometriaApp::setNome("Retangulo");
}

void DRetanguloApp::lerParametrosDaGeometria(){
    float l1,l2;
    cout<<endl<<"Digite lado1:";
    cin>>l1;
    cout<<endl<<"Digite lado2:";
    cin>>l2;
    setL1(l1);
    setL2(l2);
    cout<<endl;
}

DGeometriaApp *DRetanguloApp::alocarGeoApp() {
    return new DRetanguloApp;
}
