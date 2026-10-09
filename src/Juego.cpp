#include "raylib.h"
#include "Juego.h"
#include "Bala.h"
#include "Asteroide.h"
#include "PoliReg.h"
#include "Punto2D.h"
#include <string>

using namespace std;

void moverAstsCampo(GameState& gameState){
    auto& campoAsteroides = gameState.getCampoAsteroides();
    for(int i = 0; i < gameState.getNumAst(); i++){
        campoAsteroides[i].mover();
    }
}

void rotarAstsCampo(GameState& gameState){
    auto& campoAsteroides = gameState.getCampoAsteroides();
    for(int i = 0; i < gameState.getNumAst(); i++){
        campoAsteroides[i].rotar();
    }
}

void colisionAsts(GameState& gameState){
    auto& campoAsteroides = gameState.getCampoAsteroides();
    for(int i = 0; i < gameState.getNumAst(); i++){
        for(int j = i + 1; j < gameState.getNumAst(); j++){
            if(campoAsteroides[i].colision(campoAsteroides[j])){
                campoAsteroides[i].invertirVelocidad();
                campoAsteroides[j].invertirVelocidad();
            }
        }
    }
}

void colisionBorde(GameState& gameState){
    auto& campoAsteroides = gameState.getCampoAsteroides();
    for(int i = 0; i < gameState.getNumAst(); i++){
        float x = campoAsteroides[i].getCentro().getX();
        float y = campoAsteroides[i].getCentro().getY();
        float r = campoAsteroides[i].getRadio();

        if(x - r < 0 || x + r > gameState.getAncho()){
            campoAsteroides[i].invertirVelocidadX();
        }
        if(y - r < 0 || y + r > gameState.getAlto()){
            campoAsteroides[i].invertirVelocidadY();
        }
    }
}

void moverNaveHorizontal(GameState& gameState) {
    float velocidad = 7.0;
    auto& nave = gameState.getNave();
    float xActual = nave.getCentro().getX();
    float radio = nave.getRadio();

    Punto2D velocidadNave(0.0, 0.0);

    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        if (xActual - radio > 0) {
            velocidadNave.setXY(-velocidad, 0.0);
        }
    } else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        if (xActual + radio < gameState.getAncho()) {
            velocidadNave.setXY(velocidad, 0.0);
        }
    }

    nave.setVelocidad(velocidadNave);
    nave.mover();
}

void colisionNaveAsteroides(GameState& gameState) {
    auto& campoAsteroides = gameState.getCampoAsteroides();
    auto& nave = gameState.getNave();
    int astsVivos = gameState.getAstsVivos();

    for (int i = 0; i < astsVivos; i++) {
        if (nave.colision(campoAsteroides[i])) {
            gameState.setVidasJugador(gameState.getVidasJugador() - 1);

            campoAsteroides[i] = campoAsteroides[astsVivos - 1];
            gameState.setAstsVivos(astsVivos - 1);
            i--;

            if (gameState.getVidasJugador() <= 0) {
                gameState.setEstadoGameOver(true);
            }
        }
    }
}

void colisionesDisparos(GameState& gameState) {
    auto& disparos = gameState.getDisparos();
    auto& campoAsteroides = gameState.getCampoAsteroides();
    auto& especial = gameState.getEspecial();
    int astsVivos = gameState.getAstsVivos();
    bool finGolpe = false;

    for (int i = 0; i < gameState.getNumDisp(); i++) {
        if (disparos[i].isActiva()) {
            for (int j = 0; j < astsVivos; j++) {

                if (disparos[i].colision(campoAsteroides[j]) && !finGolpe) {
                    disparos[i].desactivar();

                    if (campoAsteroides[j].getLados() <= 3) {
                        campoAsteroides[j] = campoAsteroides[astsVivos - 1];
                        gameState.setAstsVivos(astsVivos - 1);

                        finGolpe = true;
                    } else {
                        campoAsteroides[j].esGolpeado(disparos[i]);

                        finGolpe = true;
                    }
                }
            }
        }
    }
    if (especial.isActiva()) {
        for (int j = 0; j < astsVivos; j++) {

            if (especial.colision(campoAsteroides[j])) {
                if (campoAsteroides[j].getLados() <= 3) {
                    campoAsteroides[j] = campoAsteroides[astsVivos - 1];
                    gameState.setAstsVivos(astsVivos - 1);
                    j--;

                } else {
                    for (int golpes = 0; golpes < 3; golpes++) {
                        if (campoAsteroides[j].getLados() > 3) {
                            campoAsteroides[j].esGolpeado(especial);
                        }
                    }
                }
            }
        }
    }
}

void actualizarEstado(GameState& gameState){
    moverNaveHorizontal(gameState);
    moverAstsCampo(gameState);
    rotarAstsCampo(gameState);
    colisionAsts(gameState);
    colisionBorde(gameState);

    auto& disparos = gameState.getDisparos();
    for (int i = 0; i < gameState.getNumDisp(); i++) {
        disparos[i].actualizar(0);
    }

    auto& especial = gameState.getEspecial();
    especial.actualizar(0);

    colisionesDisparos(gameState);
    colisionNaveAsteroides(gameState);
}

void pintaAsteroides(GameState& gameState){
    auto& campoAsteroides = gameState.getCampoAsteroides();
    int astsVivos = gameState.getAstsVivos();
    for(int i = 0; i < astsVivos; i++){
        Vector2 centro = { campoAsteroides[i].getCentro().getX(), campoAsteroides[i].getCentro().getY() };

        DrawPoly(centro, campoAsteroides[i].getLados(), campoAsteroides[i].getRadio(), campoAsteroides[i].getGiro(), MAROON);

        DrawPolyLines(centro, campoAsteroides[i].getLados(), campoAsteroides[i].getRadio(), campoAsteroides[i].getGiro(), BLACK);
    }
}

void pintaDisparos(GameState& gameState) {
    auto& disparos = gameState.getDisparos();
    auto& especial = gameState.getEspecial();
    for (int i = 0; i < gameState.getNumDisp(); i++) {
        if (disparos[i].isActiva()) {
            Vector2 pos = { disparos[i].getCentro().getX(), disparos[i].getCentro().getY() };
            DrawCircleV(pos, disparos[i].getRadio(), YELLOW);
        }
    }

    if (especial.isActiva()) {
        Vector2 pos = { especial.getCentro().getX(), especial.getCentro().getY() };
        DrawCircleV(pos, especial.getRadio(), ORANGE);
        DrawCircleLines((int)pos.x, (int)pos.y, especial.getRadio() + 2, RED);
    }
}

void pintaNave(GameState& gameState){
    auto& nave = gameState.getNave();
    Vector2 centro = { nave.getCentro().getX(), nave.getCentro().getY() };
    DrawPoly(centro, nave.getLados(), nave.getRadio(), nave.getGiro(), RED);
}

void reiniciarJuego(GameState& gameState) {
    gameState.reiniciar();
}

int main(){
        GameState gameState;
        InitWindow(gameState.getAncho(), gameState.getAlto(), "Proyecto Final MP");
        bool pantallaTitulo = true;
        SetTargetFPS(60);

        gameState.getEspecial().disparoEspecial();

        while(!WindowShouldClose()){
            if(pantallaTitulo){
                if(IsKeyPressed(KEY_ENTER)){
                    reiniciarJuego(gameState);
                    pantallaTitulo = false;
                }
            } else if (gameState.getEstadoGameOver() || gameState.getEstadoVictoria()) {
                if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_R)) {
                    reiniciarJuego(gameState);
                }
            } else {
                auto& nave = gameState.getNave();
                auto& especial = gameState.getEspecial();
                auto& disparos = gameState.getDisparos();
                float posXNave = nave.getCentro().getX();
                float posYNave = nave.getCentro().getY();

                if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
                    gameState.setTiempoCarga(gameState.getTiempoCarga() + GetFrameTime());

                    if (gameState.getTiempoCarga() >= gameState.getTiempoEspecial() && !especial.isActiva()) {
                        especial.disparar(posXNave, posYNave);
                        gameState.setTiempoCarga(0.0);
                    }
                } else if (IsKeyReleased(KEY_W) || IsKeyReleased(KEY_UP)) {
                    if (gameState.getTiempoCarga() > 0.0 && gameState.getTiempoCarga() < gameState.getTiempoEspecial()) {
                        bool disparado = false;
                        for (int i = 0; i < gameState.getNumDisp(); i++) {
                            if (!disparos[i].isActiva() && !disparado) {
                                disparos[i].disparar(posXNave, posYNave);
                                disparado = true;
                            }
                        }
                    }
                    gameState.setTiempoCarga(0.0);
                }
                actualizarEstado(gameState);

                if (gameState.getAstsVivos() <= 0) {
                    gameState.setEstadoVictoria(true);
                }
            }

            BeginDrawing();
            ClearBackground(RAYWHITE);

            if(pantallaTitulo){
                DrawText("Asteroids but Roguelike", 200, 120, 20, RED);
                DrawText("Press Enter", 200, 160, 20, BLUE);
                DrawText("v0.1, Dr. Plasmadrid", 200, 180, 20, BLACK);
            } else if (gameState.getEstadoGameOver()) {
                const char* txtMuerte = "GAME OVER";
                const char* txtReinicio = "Press R o ENTER to retry";

                int anchoTxt1 = MeasureText(txtMuerte, 40);
                int anchoTxt2 = MeasureText(txtReinicio, 20);

                DrawText(txtMuerte, (gameState.getAncho() / 2) - (anchoTxt1 / 2), (gameState.getAlto() / 2) - 40, 40, RED);
                DrawText(txtReinicio, (gameState.getAncho() / 2) - (anchoTxt2 / 2), (gameState.getAlto() / 2) + 10, 20, DARKGRAY);
            } else if (gameState.getEstadoVictoria()) {
                const char* txtVictoria = "YOU WIN!";
                const char* txtReinicio = "Press R o ENTER to play again";

                int anchoTxt1 = MeasureText(txtVictoria, 40);
                int anchoTxt2 = MeasureText(txtReinicio, 20);

                DrawText(txtVictoria, (gameState.getAncho() / 2) - (anchoTxt1 / 2), (gameState.getAlto() / 2) - 40, 40, GREEN);
                DrawText(txtReinicio, (gameState.getAncho() / 2) - (anchoTxt2 / 2), (gameState.getAlto() / 2) + 10, 20, DARKGRAY);

            } else {
                string s = "Asteroids: " + to_string(gameState.getAstsVivos());
                DrawText(s.c_str(), 10, 10, 20, BLACK);

                string textoVidas = "";
                for(int i = 0; i < gameState.getVidasJugador(); i++) {
                    textoVidas += "<3 ";
                }
                int anchoVidas = MeasureText(textoVidas.c_str(), 20);
                DrawText(textoVidas.c_str(), gameState.getAncho() - anchoVidas - 10, 10, 20, RED);

                if (gameState.getTiempoCarga() > 0) {
                    float porcentaje = gameState.getTiempoCarga() / gameState.getTiempoEspecial();
                    if (porcentaje > 1.0){
                        porcentaje = 1.0;
                    }

                    auto& nave = gameState.getNave();
                    DrawRectangle(nave.getCentro().getX() - 20, nave.getCentro().getY() + 30, 40 * porcentaje, 5, ORANGE);

                    if (porcentaje >= 1.0) {
                        DrawText("Boom!", nave.getCentro().getX() - 35, nave.getCentro().getY() + 40, 15, RED);
                    }
                }

                pintaAsteroides(gameState);
                pintaDisparos(gameState);
                pintaNave(gameState);
            }

            EndDrawing();
        }

        CloseWindow();
        return 0;
}