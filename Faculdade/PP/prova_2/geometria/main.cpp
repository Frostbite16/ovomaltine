#include <iostream>
#include <vector>
#include "DAppCalcArea.h"

using namespace std;

int main() {
    DAppCalcArea app;
    DQuadradoApp q1;
    DRetanguloApp r1;

    app.registraGeometria(&q1);
    app.registraGeometria(&r1);

    app.mainDoApp();
    return 0;
}
