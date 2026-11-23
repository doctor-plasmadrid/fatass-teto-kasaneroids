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
    float tiempoCarga2;
    int uAst;
    int uDisp;
    int vidasJugador;
    int vidasJugador2;
    bool estadoGameOver;
    bool estadoVictoria;
    bool modoInfinito;
    bool dosJugadores;
    int nivel;
    int personajeSeleccionado;
    int personajeSeleccionado2;

    std::vector<Asteroide> campoAsteroides;
    std::vector<Bala> disparos;
    std::vector<Bala> disparos2;
    Bala especial;
    Bala especial2;
    Asteroide nave;
    Asteroide nave2;

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
    float& getTiempoCarga() { return tiempoCarga; }
    float getTiempoCarga2() const { return tiempoCarga2; }
    float& getTiempoCarga2() { return tiempoCarga2; }
    int getVidasJugador() const { return vidasJugador; }
    int getVidasJugador2() const { return vidasJugador2; }
    bool getEstadoGameOver() const { return estadoGameOver; }
    bool getEstadoVictoria() const { return estadoVictoria; }
    bool getModoInfinito() const { return modoInfinito; }
    int getNivel() const { return nivel; }
    int getPersonajeSeleccionado() const { return personajeSeleccionado; }
    int getPersonajeSeleccionado2() const { return personajeSeleccionado2; }

    Asteroide& getNave() { return nave; }
    const Asteroide& getNave() const { return nave; }
    Asteroide& getNave2() { return nave2; }
    const Asteroide& getNave2() const { return nave2; }
    Bala& getEspecial() { return especial; }
    const Bala& getEspecial() const { return especial; }
    Bala& getEspecial2() { return especial2; }
    const Bala& getEspecial2() const { return especial2; }

    std::vector<Asteroide>& getCampoAsteroides() { return campoAsteroides; }
    const std::vector<Asteroide>& getCampoAsteroides() const { return campoAsteroides; }
    std::vector<Bala>& getDisparos() { return disparos; }
    const std::vector<Bala>& getDisparos() const { return disparos; }
    std::vector<Bala>& getDisparos2() { return disparos2; }
    const std::vector<Bala>& getDisparos2() const { return disparos2; }

    // Setters
    void setAstsVivos(int value) { astsVivos = value; }
    void setTiempoCarga(float value) { tiempoCarga = value; }
    void setTiempoCarga2(float value) { tiempoCarga2 = value; }
    void setVidasJugador(int value) { vidasJugador = value; }
    void setVidasJugador2(int value) { vidasJugador2 = value; }
    void setEstadoGameOver(bool value) { estadoGameOver = value; }
    void setEstadoVictoria(bool value) { estadoVictoria = value; }
    void setModoInfinito(bool value) { modoInfinito = value; }
    void setDosJugadores(bool value) { dosJugadores = value; }
    void setNivel(int value) { nivel = value; }
    void setPersonajeSeleccionado(int value) { personajeSeleccionado = value; }
    void setPersonajeSeleccionado2(int value) { personajeSeleccionado2 = value; }

    // Methods
    void reiniciar();
    void siguienteNivel();
};

#endif
