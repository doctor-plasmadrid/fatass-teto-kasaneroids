#ifndef POLIREG_H
#define POLIREG_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "Punto2D.h"

using namespace std;

class PoliReg{
    private:
        static const int MAX_VERT;
        static const int MIN_VERT;
        static const float MAX_RAD;
        static const float MIN_RAD;

        vector<Punto2D> vertices;
        Punto2D centro;
        float radio;

        void generaVertices();
        void copia(const PoliReg &otro);
        void copiaParam(int nVert, const Punto2D & centro_otro, float r);
        bool diferenciaAbs(const PoliReg &otro) const;

    public:
        PoliReg();
        PoliReg(int nVert, const Punto2D & centro, float r);
        PoliReg(const PoliReg & otro);
        void operator=(const PoliReg & otro);
        bool operator<(const PoliReg & otro) const;
        bool operator==(const PoliReg & otro) const;
        bool operator!=(const PoliReg & otro) const;
        friend ostream& operator<<(ostream &flujo, const PoliReg &pReg);
        friend istream& operator>>(istream &flujo, PoliReg &pReg);
        Punto2D getCentro() const;
        float getRadio() const;
        Punto2D getVertice(const int &numVert) const;
        int getLados() const;
        void setCentro(const Punto2D &nuevoCentro);
        void agregaVertice();
        void eliminaVertice();
        void expande(float delta);
        void contrae(float delta);
        double perimetro() const;
        void rotar(float rads);
        bool colision(const PoliReg & otro) const;
        void mover(float dx, float dy);
        string toString();

};

#endif