//
// Created by delair on 05/09/24.
//

#ifndef GEOMETRIA_DCALC_H
#define GEOMETRIA_DCALC_H
#include "DGeometria.h"

class DCalc{
    vector<DGeometria*> geos;
public:
    void insert(DGeometria*);
    float areTotal();
    ~DCalc();
};


#endif //GEOMETRIA_DCALC_H
