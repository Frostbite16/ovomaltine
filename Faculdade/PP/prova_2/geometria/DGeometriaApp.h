//
// Created by delair on 05/09/24.
//

#ifndef GEOMETRIA_DGEOMETRIAAPP_H
#define GEOMETRIA_DGEOMETRIAAPP_H
#include <string>
#include "DGeometria.h"
using namespace std;
class DGeometriaApp {
    string nome;
public:
    const string &getNome() const;
    void setNome(const string &nome);
    virtual void lerParametrosDaGeometria()=0;
    virtual DGeometriaApp* alocarGeoApp()=0;
};
class DQuadradoApp:private DQuadrado,public DGeometriaApp{
public:
    DQuadradoApp();
    void lerParametrosDaGeometria();

    DGeometriaApp *alocarGeoApp();
};
class DRetanguloApp:private DRetangulo,public DGeometriaApp{
public:
    DRetanguloApp();
    void lerParametrosDaGeometria();

    DGeometriaApp *alocarGeoApp();
};


#endif //GEOMETRIA_DGEOMETRIAAPP_H
