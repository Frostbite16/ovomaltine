//
// Created by delair on 05/09/24.
//

#ifndef GEOMETRIA_DAPPCALCAREA_H
#define GEOMETRIA_DAPPCALCAREA_H

#include <vector>
#include "DCalc.h"
#include "DGeometriaApp.h"
using namespace std;
class DAppCalcArea {
    vector <DGeometriaApp*> registrosDeGeometrias;
    DCalc calc;

public:
    void registraGeometria(DGeometriaApp*);
    void mainDoApp();
    ~DAppCalcArea();
};

#endif //GEOMETRIA_DAPPCALCAREA_H
