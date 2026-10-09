#include "GameState.h"
#include <cstdlib>

GameState::GameState()
    : astsVivos(NUM_AST), tiempoCarga(0.0), uAst(0), uDisp(0),
      vidasJugador(3), estadoGameOver(false), estadoVictoria(false),
      campoAsteroides(NUM_AST), disparos(NUM_DISP) {
}

void GameState::reiniciar() {
    vidasJugador = 3;
    astsVivos = NUM_AST;
    estadoGameOver = false;
    estadoVictoria = false;

    nave.setPosicion(ANCHO / 2.0, ALTO - 15.0);

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
    }

    especial.desactivar();
}
