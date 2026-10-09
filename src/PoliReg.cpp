#include "PoliReg.h"
#include "Punto2D.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <cmath>
using namespace std;

const float PoliReg::MAX_RAD = 200.0;
const float PoliReg::MIN_RAD = 15.0;
const int PoliReg::MAX_VERT = 200;
const int PoliReg::MIN_VERT = 3;



void PoliReg::copia(const PoliReg & otro){
    vertices = otro.vertices;
    centro = otro.centro;
    radio = otro.radio;
};

void PoliReg::copiaParam(int nVert, const Punto2D & centro_otro, float r){
    centro = centro_otro;
    radio = r;

    while(static_cast<int>(vertices.size()) < nVert && static_cast<int>(vertices.size()) < MAX_VERT){
        agregaVertice();
    }
};

void PoliReg::generaVertices(){
    if(!vertices.empty()){
        float alfa = (360.0/static_cast<float>(vertices.size()))*(M_PI/180.0);
        vertices[0].setXY(centro.getX() + radio, centro.getY());

        for(size_t i = 1; i < vertices.size(); i++){
            vertices[i] = vertices[i-1];
            vertices[i].rotar(centro, alfa);
        }
    }
};

bool PoliReg::diferenciaAbs(const PoliReg & otro) const{
    bool devolver = true;

    if(vertices.size() != otro.vertices.size()){
        devolver = false;
    };

    for(size_t i = 0; i < vertices.size() && devolver; i++){
        if(vertices[i] == otro.vertices[i]){
            devolver = false;
        };
    };

    return devolver;
};

PoliReg::PoliReg(){
    vertices.resize(MIN_VERT);
    centro.setXY(0.0,0.0);
    radio = MIN_RAD;
    generaVertices();
};

PoliReg::PoliReg(const PoliReg & otro){
    copia(otro);
};

PoliReg::PoliReg(int nVert, const Punto2D & centro_otro, float r){
    if (nVert < MIN_VERT) {
        nVert = MIN_VERT;
    } else if (nVert > MAX_VERT) {
        nVert = MAX_VERT;
    };

    if (r < MIN_RAD) {
        r = MIN_RAD;
    } else if (r > MAX_RAD) {
        r = MAX_RAD;
    };

    vertices.resize(MIN_VERT);
    centro.setXY(0.0,0.0);
    radio = MIN_RAD;
    generaVertices();
    copiaParam(nVert, centro_otro, r);
};

void PoliReg::operator=(const PoliReg & otro){
    if (this != &otro){
        vertices = otro.vertices;
        centro = otro.centro;
        radio = otro.radio;
    };
};

bool PoliReg::operator<(const PoliReg & otro) const{
    bool devolver = false;

    if(abs(perimetro() - otro.perimetro()) < 0.0001 && this != &otro){
        devolver = true;
    }

    return devolver;
};

bool PoliReg::operator==(const PoliReg & otro) const{
    return (centro == otro.getCentro() && radio == otro.getRadio() && diferenciaAbs(otro));
};

bool PoliReg::operator!=(const PoliReg & otro) const{
    return !(*this == otro);
};

ostream& operator<<(ostream &flujo, const PoliReg &pReg){
    flujo << "@centro ";
    flujo << pReg.centro << endl;
    flujo << "@radio ";
    flujo << pReg.radio << endl;
    flujo << "@Puntos ";
    flujo << pReg.vertices.size() << endl;

    for(size_t i = 0; i < pReg.vertices.size(); i++){
        flujo << pReg.vertices[i] << "\n";
    }

    return flujo;
};

istream& operator>>(istream &flujo, PoliReg &pReg){
    Punto2D cent;
    float rad;
    int pts;

    flujo >> cent;
    flujo >> rad;
    flujo >> pts;

    PoliReg aux(pts, cent, rad);

    for(int i = 0; i < pts; i++){
        flujo >> aux.vertices[i];
    }

    pReg = aux;

    return flujo;
}

void PoliReg::setCentro(const Punto2D &nuevoCentro){
    centro = nuevoCentro;
    generaVertices();
}

void PoliReg::agregaVertice(){
    if(static_cast<int>(vertices.size()) < MAX_VERT){
        vertices.push_back(Punto2D());
        generaVertices();
    }
};

void PoliReg::eliminaVertice(){
    if(static_cast<int>(vertices.size()) > MIN_VERT){
        vertices.pop_back();
        generaVertices();
    }
};

Punto2D PoliReg::getCentro() const{
    return centro;
};

float PoliReg::getRadio() const{;
    return radio;
};

Punto2D PoliReg::getVertice(const int &numVert) const{
    return vertices[numVert];
};

int PoliReg::getLados() const{
    return static_cast<int>(vertices.size());
};

void PoliReg::expande(float delta){
    bool validez = ((radio + delta) >= MIN_RAD && (radio + delta) <= MAX_RAD);
    if(validez){
        radio += delta;
        generaVertices();
    }
};

void PoliReg::contrae(float delta){
    expande(delta*-1.0);
};

double PoliReg::perimetro() const{
    if(vertices.size() < 2) return 0.0;
    return static_cast<double>(vertices.size()) * (vertices[0].distancia(vertices[1]));
};

void PoliReg::rotar(float rads){
    for(size_t i = 0; i < vertices.size(); i++){
        vertices[i].rotar(centro, rads);
    }
};

bool PoliReg::colision(const PoliReg & otro) const{
    bool colisionan = false;

    if(this != &otro){
        colisionan = centro.distancia(otro.getCentro()) < (radio + otro.getRadio()) + 0.1;
    };

    return colisionan;
};

void PoliReg::mover(float dx, float dy){
    centro.mover(dx, dy);
    for(size_t i = 0; i < vertices.size(); i++){
        vertices[i].mover(dx,dy);
    };
};

string PoliReg::toString(){
    string strcentrox = to_string(centro.getX());
    string strcentroy = to_string(centro.getY());
    string strradio = to_string(radio);
    string strvert = to_string(vertices.size());

    string devolver = ("@centro " + strcentrox + " " + strcentroy + "\n");
    devolver += ("@radio " + strradio + "\n");
    devolver += ("@vertices " + strvert + "\n");
    for(size_t i = 0; i < vertices.size(); i++){
        devolver += (vertices[i].toString() + "\n");
    }

    return devolver;
};