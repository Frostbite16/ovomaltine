//
// Created by delair on 05/09/24.
//

#include "DCalc.h"
void DCalc::insert(DGeometria *pGeo) {
    geos.push_back(pGeo->alocar());
}

float DCalc::areTotal() {
    vector<DGeometria *>::iterator it;
    float area = 0.0;
    for (it = geos.begin(); it != geos.end(); ++it)
        area += (*it)->calcArea();
    return area;
}

DCalc::~DCalc() {
    vector<DGeometria *>::iterator it;
    for (it = geos.begin(); it != geos.end(); ++it)
        delete *it;
}