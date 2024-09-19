//
// Created by delair on 05/09/24.
//

#ifndef GEOMETRIA_DGEOMETRIA_H
#define GEOMETRIA_DGEOMETRIA_H
#include <vector>

using namespace std;

class DGeometria{
public:
    virtual float calcArea()=0;
    virtual DGeometria* alocar()=0;
    virtual ~DGeometria();
};
class DQuadrado: public DGeometria{
    float lado;
public:
    DQuadrado(float lado=1.0);
    virtual float calcArea();
    DGeometria *alocar();

    float getLado() const;
    void setLado(float lado);
};
class DRetangulo:public DGeometria{
    float l1,l2;
public:
    DRetangulo(float l1 = 1, float l2 = 1);
    virtual float calcArea();
    DGeometria *alocar();

    float getL1() const;
    void setL1(float l1);
    float getL2() const;
    void setL2(float l2);
};


#endif //GEOMETRIA_DGEOMETRIA_H
