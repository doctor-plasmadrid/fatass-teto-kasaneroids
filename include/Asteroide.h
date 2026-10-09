#ifndef ASTEROIDE_H
#define ASTEROIDE_H
#include "PoliReg.h"
#include "Punto2D.h"
#include <string>

using namespace std;

class Asteroide{
    private:
        static const int MAX_X;
        static const int MAX_Y;
        static const int MAX_VERT;
        static const int MIN_VERT;
        static const int MAX_AST;

        PoliReg roca;
        Punto2D veloc;
        float giro;
    public:
        Asteroide();
        Asteroide(const PoliReg &pol, const Punto2D &vel, float rads=10);
        Punto2D getCentro();
        float getRadio();
        int getLados();
        float getGiro();
        void setPosicion(float x, float y);
        void mover();
        void rotar();
        bool colision(const Asteroide & otro);
        void setVelocidad(const Punto2D &nuevaVel);
        void invertirVelocidadX();
        void invertirVelocidadY();
        void invertirVelocidad();
        void multiplicarTamanio(float veces);
        bool esGolpeado(const Asteroide &nave);
};

#endif