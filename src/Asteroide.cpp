#include "Asteroide.h"
#include "PoliReg.h"
#include "Punto2D.h"
#include <string>
using namespace std;

const int Asteroide::MAX_X = 640;
const int Asteroide::MAX_Y = 480;
const int Asteroide::MAX_VERT = 7;
const int Asteroide::MIN_VERT = 3;
const int Asteroide::MAX_AST = 10;

Asteroide::Asteroide(){
    int verts = rand() % (MAX_VERT - MIN_VERT + 1);

    for(int i = 0; i < verts; i++){
        roca.agregaVertice();
    };

    float velX = ((rand() % 41) - 20) / 10.0; 
    float velY = ((rand() % 41) - 20) / 10.0;

    setVelocidad(Punto2D(velX, velY));

    giro = rand() % 360;
};

Asteroide::Asteroide(const PoliReg &pol, const Punto2D &vel, float rads){
    roca = pol;

    if(roca.getLados() > MAX_VERT){
        for(int i = pol.getLados(); i > MAX_VERT; i--){
            roca.eliminaVertice();
        };
    };
   
    veloc = vel;
    giro = rads;
};

Punto2D Asteroide::getCentro(){
    return roca.getCentro();
}

int Asteroide::getLados(){
    return roca.getLados();
}

float Asteroide::getGiro(){
    return giro;
}

float Asteroide::getRadio(){
    return roca.getRadio();
}

void Asteroide::setPosicion(float x, float y) {
    roca.setCentro(Punto2D(x, y)); 
}

void Asteroide::mover(){
    roca.mover(veloc.getX(),veloc.getY());
};

void Asteroide::rotar(){
    roca.rotar(giro);
};

bool Asteroide::colision(const Asteroide & otro){
    return roca.colision(otro.roca);
};

void Asteroide::setVelocidad(const Punto2D &nuevaVel){
    veloc = nuevaVel;
};

void Asteroide::invertirVelocidadX(){
    veloc.setXY(veloc.getX()*-1.0,veloc.getY());
};

void Asteroide::invertirVelocidadY(){
    veloc.setXY(veloc.getX(),veloc.getY()*-1.0);
};

void Asteroide::invertirVelocidad(){
    veloc.setXY(veloc.getX()*-1.0,veloc.getY()*-1.0);
};

void Asteroide::multiplicarTamanio(float veces){
    roca.expande(roca.getRadio()*veces);
};

bool Asteroide::esGolpeado(const Asteroide &bala){
    bool devolver = false;

    if(colision(bala)){
        if(roca.getLados() < MIN_VERT){
            devolver = true;
        }else{
            roca.eliminaVertice();
        }
    }

    return devolver;
}