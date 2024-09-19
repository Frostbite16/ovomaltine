//
// Created by delair on 05/09/24.
//

#include "DGeometria.h"

DGeometria::~DGeometria()
{}

DQuadrado::DQuadrado(float lado):lado(lado)
{}

float
DQuadrado::calcArea() {
    return lado * lado;
}

DGeometria *DQuadrado::alocar() {
    return new DQuadrado(this->lado);
}

float DQuadrado::getLado() const {
    return lado;
}

void DQuadrado::setLado(float lado) {
    DQuadrado::lado = lado;
}

DRetangulo::DRetangulo(float l1, float l2) : l1(l1), l2(l2)
{}

float
DRetangulo::calcArea() {
    return l1 * l2;
}

DGeometria *DRetangulo::alocar() {
    return new DRetangulo(this->l1,this->l2);
}

float DRetangulo::getL1() const {
    return l1;
}

void DRetangulo::setL1(float l1) {
    DRetangulo::l1 = l1;
}

float DRetangulo::getL2() const {
    return l2;
}

void DRetangulo::setL2(float l2) {
    DRetangulo::l2 = l2;
}
