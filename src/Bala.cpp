#include "Bala.h"
#include "Punto2D.h"
#include "PoliReg.h"

Bala::Bala() {
    activa = false;
    
    setVelocidad(Punto2D(0.0, -7.0));
}

void Bala::disparar(float posX, float posY) {
    setPosicion(posX, posY);
    activa = true;
}

void Bala::disparoEspecial() {
    setVelocidad(Punto2D(0.0, -4.0)); 
    
    // NOTA: Como la variable 'roca' está oculta en Asteroide,
    // asegúrate de tener una forma de cambiar su tamaño. 
    // Si no la tienes, añade un método 'multiplicarTamaño(float factor)' en Asteroide
    // que llame a roca.expande(...) o que cambie su radio.
}

void Bala::actualizar(int techo) {
    if (activa){
        mover(); 

        if (getCentro().getY() + getRadio() < techo) {
            desactivar();
        }
    }
}

bool Bala::isActiva() const {
    return activa;
}

void Bala::desactivar() {
    activa = false;
}