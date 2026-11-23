#include "GameState.h"
#include <cstdlib>

GameState::GameState()
    : astsVivos(NUM_AST), tiempoCarga(0.0), tiempoCarga2(0.0), uAst(0), uDisp(0),
      vidasJugador(3), vidasJugador2(3), estadoGameOver(false), estadoVictoria(false),
      modoInfinito(false), dosJugadores(false), nivel(1), personajeSeleccionado(0), personajeSeleccionado2(0),
      campoAsteroides(NUM_AST), disparos(NUM_DISP), disparos2(NUM_DISP) {
}

void GameState::reiniciar() {
    vidasJugador = 3;
    vidasJugador2 = dosJugadores ? 3 : 0;
    astsVivos = NUM_AST;
    estadoGameOver = false;
    estadoVictoria = false;
    nivel = 1;
    tiempoCarga = 0.0;
    tiempoCarga2 = 0.0;

    nave.setPosicion(ANCHO * 0.35, ALTO - 15.0);
    nave2.setPosicion(ANCHO * 0.65, ALTO - 15.0);

    for (int i = 0; i < NUM_AST; i++) {
        int r = static_cast<int>(campoAsteroides[i].getRadio());

        int minX = r;
        int maxX = ANCHO - r;

        int minY = r;
        int maxY = (ALTO * 0.8) - r;

        int posX = minX + (rand() % (maxX - minX + 1));
        int posY = minY + (rand() % (maxY - minY + 1));

        campoAsteroides[i].setPosicion(posX, posY);
    }

    for (int i = 0; i < NUM_DISP; i++) {
        disparos[i].desactivar();
        disparos2[i].desactivar();
    }

    especial.desactivar();
    especial2.desactivar();
}

void GameState::siguienteNivel() {
    nivel++;
    vidasJugador++;
    if (dosJugadores) {
        vidasJugador2++;
    }
    astsVivos = NUM_AST;
    tiempoCarga = 0.0;
    tiempoCarga2 = 0.0;

    nave.setPosicion(ANCHO * 0.35, ALTO - 15.0);
    nave2.setPosicion(ANCHO * 0.65, ALTO - 15.0);

    for (int i = 0; i < NUM_AST; i++) {
        int r = static_cast<int>(campoAsteroides[i].getRadio());

        int minX = r;
        int maxX = ANCHO - r;

        int minY = r;
        int maxY = (ALTO * 0.8) - r;

        int posX = minX + (rand() % (maxX - minX + 1));
        int posY = minY + (rand() % (maxY - minY + 1));

        campoAsteroides[i].setPosicion(posX, posY);
    }

    for (int i = 0; i < NUM_DISP; i++) {
        disparos[i].desactivar();
        disparos2[i].desactivar();
    }

    especial.desactivar();
    especial2.desactivar();
}
