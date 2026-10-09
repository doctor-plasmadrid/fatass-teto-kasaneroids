#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Asteroide.h"
#include "Bala.h"
#include <vector>

class GameState {
private:
    static const int ANCHO = 640;
    static const int ALTO = 480;
    static const int NUM_AST = 10;
    static const int NUM_DISP = 3;
    static constexpr double intervaloDisparo = 3.0;
    static constexpr float TIEMPO_ESPECIAL = 1.0;

    int astsVivos;
    float tiempoCarga;
    int uAst;
    int uDisp;
    int vidasJugador;
    bool estadoGameOver;
    bool estadoVictoria;

    std::vector<Asteroide> campoAsteroides;
    std::vector<Bala> disparos;
    Bala especial;
    Asteroide nave;

public:
    GameState();

    // Getters
    int getAncho() const { return ANCHO; }
    int getAlto() const { return ALTO; }
    int getNumAst() const { return NUM_AST; }
    int getNumDisp() const { return NUM_DISP; }
    double getIntervaloDisparo() const { return intervaloDisparo; }
    float getTiempoEspecial() const { return TIEMPO_ESPECIAL; }

    int getAstsVivos() const { return astsVivos; }
    float getTiempoCarga() const { return tiempoCarga; }
    int getVidasJugador() const { return vidasJugador; }
    bool getEstadoGameOver() const { return estadoGameOver; }
    bool getEstadoVictoria() const { return estadoVictoria; }

    Asteroide& getNave() { return nave; }
    const Asteroide& getNave() const { return nave; }
    Bala& getEspecial() { return especial; }
    const Bala& getEspecial() const { return especial; }

    std::vector<Asteroide>& getCampoAsteroides() { return campoAsteroides; }
    const std::vector<Asteroide>& getCampoAsteroides() const { return campoAsteroides; }
    std::vector<Bala>& getDisparos() { return disparos; }
    const std::vector<Bala>& getDisparos() const { return disparos; }

    // Setters
    void setAstsVivos(int value) { astsVivos = value; }
    void setTiempoCarga(float value) { tiempoCarga = value; }
    void setVidasJugador(int value) { vidasJugador = value; }
    void setEstadoGameOver(bool value) { estadoGameOver = value; }
    void setEstadoVictoria(bool value) { estadoVictoria = value; }

    // Methods
    void reiniciar();
};

#endif
