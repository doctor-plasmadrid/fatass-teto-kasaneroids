#ifndef BALA_H
#define BALA_H

#include "Asteroide.h"

class Bala : public Asteroide {
private:
    bool activa;

public:
    Bala();

    void disparar(float posX, float posY);
    void actualizar(int limiteTecho);
    bool isActiva() const;
    void desactivar();
    void disparoEspecial();
};

#endif