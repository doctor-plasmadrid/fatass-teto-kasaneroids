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
    int astsVivos = gameState.getAstsVivos();
    for(int i = 0; i < astsVivos; i++){
        campoAsteroides[i].mover();
    }
}

void rotarAstsCampo(GameState& gameState){
    auto& campoAsteroides = gameState.getCampoAsteroides();
    int astsVivos = gameState.getAstsVivos();
    for(int i = 0; i < astsVivos; i++){
        campoAsteroides[i].rotar();
    }
}

void colisionAsts(GameState& gameState){
    auto& campoAsteroides = gameState.getCampoAsteroides();
    int astsVivos = gameState.getAstsVivos();
    for(int i = 0; i < astsVivos; i++){
        for(int j = i + 1; j < astsVivos; j++){
            if(campoAsteroides[i].colision(campoAsteroides[j])){
                campoAsteroides[i].invertirVelocidad();
                campoAsteroides[j].invertirVelocidad();
            }
        }
    }
}

void colisionBorde(GameState& gameState){
    auto& campoAsteroides = gameState.getCampoAsteroides();
    int astsVivos = gameState.getAstsVivos();
    for(int i = 0; i < astsVivos; i++){
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
            astsVivos--;
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
                        astsVivos--;
                        j--;

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
                    astsVivos--;
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

void pintaAsteroides(GameState& gameState, Texture2D& tetoTexture){
    auto& campoAsteroides = gameState.getCampoAsteroides();
    int astsVivos = gameState.getAstsVivos();
    Color maroonAlpha = { MAROON.r, MAROON.g, MAROON.b, 64 };
    Color blackAlpha = { BLACK.r, BLACK.g, BLACK.b, 64 };

    for(int i = 0; i < astsVivos; i++){
        Vector2 centro = { campoAsteroides[i].getCentro().getX(), campoAsteroides[i].getCentro().getY() };
        float radio = campoAsteroides[i].getRadio();

        float scale = ((radio * 2.0f) / tetoTexture.width) * 3.0f;
        Vector2 texturePos = { centro.x - (radio * 3.0f), centro.y - (radio * 3.0f) };

        DrawTextureEx(tetoTexture, texturePos, 0.0f, scale, WHITE);

        DrawPoly(centro, campoAsteroides[i].getLados(), radio, campoAsteroides[i].getGiro(), maroonAlpha);

        DrawPolyLines(centro, campoAsteroides[i].getLados(), radio, campoAsteroides[i].getGiro(), blackAlpha);
    }
}

void pintaDisparos(GameState& gameState, Texture2D& baguetteTexture) {
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
        float radio = especial.getRadio();

        float scale = ((radio * 2.0f) / baguetteTexture.width) * 3.0f;
        Vector2 texturePos = { pos.x - (radio * 3.0f), pos.y - (radio * 3.0f) };

        DrawTextureEx(baguetteTexture, texturePos, 0.0f, scale, WHITE);
    }
}

void pintaNave(GameState& gameState, Texture2D& peartoTexture, Texture2D& mikuTexture, Texture2D& neruTexture, Texture2D& lukaTexture, Texture2D& kaitoTexture, Texture2D& gumiTexture){
    auto& nave = gameState.getNave();
    Vector2 centro = { nave.getCentro().getX(), nave.getCentro().getY() };
    float radio = nave.getRadio();

    Texture2D* selectedTexture;
    switch (gameState.getPersonajeSeleccionado()) {
        case 0: selectedTexture = &peartoTexture; break;
        case 1: selectedTexture = &mikuTexture; break;
        case 2: selectedTexture = &neruTexture; break;
        case 3: selectedTexture = &lukaTexture; break;
        case 4: selectedTexture = &kaitoTexture; break;
        case 5: selectedTexture = &gumiTexture; break;
        default: selectedTexture = &peartoTexture; break;
    }

    float targetHeight = radio * 2.0f * 2.5f;
    float scale = targetHeight / selectedTexture->height;
    float scaledWidth = selectedTexture->width * scale;
    float scaledHeight = selectedTexture->height * scale;

    Vector2 texturePos = { centro.x - (scaledWidth / 2.0f), centro.y - scaledHeight };

    DrawTextureEx(*selectedTexture, texturePos, 0.0f, scale, WHITE);
}

void reiniciarJuego(GameState& gameState) {
    gameState.reiniciar();
}

int main(){
        GameState gameState;
        InitWindow(gameState.getAncho(), gameState.getAlto(), "Fatass Teto Kasaneroids");
        bool pantallaTitulo = true;
        bool pantallaSeleccion = false;
        bool pantallaPersonaje = false;
        int opcionSeleccionada = 0;
        int personajeSeleccionado = 0;
        SetTargetFPS(60);

        Texture2D tetoTexture = LoadTexture("assets/fatass_teto.png");
        Texture2D peartoTexture = LoadTexture("assets/pearto.png");
        Texture2D mikuTexture = LoadTexture("assets/miku.png");
        Texture2D neruTexture = LoadTexture("assets/neru.png");
        Texture2D lukaTexture = LoadTexture("assets/luka.png");
        Texture2D kaitoTexture = LoadTexture("assets/kaito.png");
        Texture2D gumiTexture = LoadTexture("assets/gumi.png");
        Texture2D baguetteTexture = LoadTexture("assets/baguette.png");

        gameState.getEspecial().disparoEspecial();

        while(!WindowShouldClose()){
            if(pantallaTitulo){
                if(IsKeyPressed(KEY_ENTER)){
                    pantallaTitulo = false;
                    pantallaSeleccion = true;
                }
            } else if (pantallaSeleccion) {
                if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
                    opcionSeleccionada = 0;
                } else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
                    opcionSeleccionada = 1;
                } else if (IsKeyPressed(KEY_ENTER)) {
                    pantallaSeleccion = false;
                    pantallaPersonaje = true;
                }
            } else if (pantallaPersonaje) {
                if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
                    personajeSeleccionado = (personajeSeleccionado - 1 + 6) % 6;
                } else if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
                    personajeSeleccionado = (personajeSeleccionado + 1) % 6;
                } else if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
                    personajeSeleccionado = (personajeSeleccionado - 3 + 6) % 6;
                } else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
                    personajeSeleccionado = (personajeSeleccionado + 3) % 6;
                } else if (IsKeyPressed(KEY_ENTER)) {
                    gameState.setModoInfinito(opcionSeleccionada == 1);
                    gameState.setPersonajeSeleccionado(personajeSeleccionado);
                    reiniciarJuego(gameState);
                    pantallaPersonaje = false;
                }
            } else if (gameState.getEstadoGameOver()) {
                if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_R)) {
                    reiniciarJuego(gameState);
                }
            } else if (gameState.getEstadoVictoria()) {
                if (gameState.getModoInfinito()) {
                    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_R)) {
                        gameState.siguienteNivel();
                        gameState.setEstadoVictoria(false);
                    }
                } else {
                    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_R)) {
                        reiniciarJuego(gameState);
                    }
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
                DrawText("Fatass Teto Kasaneroids", 200, 120, 20, RED);
                DrawText("Press Enter", 200, 160, 20, BLUE);
                DrawText("v0.1, Dr. Plasmadrid", 200, 180, 20, BLACK);
            } else if (pantallaSeleccion) {
                DrawText("Select Game Mode", 200, 150, 20, BLACK);

                Rectangle btnSingle = { 170, 200, 300, 50 };
                Rectangle btnInfinito = { 170, 280, 300, 50 };

                Color btnColorSingle = opcionSeleccionada == 0 ? DARKGRAY : LIGHTGRAY;
                Color btnColorInfinito = opcionSeleccionada == 1 ? DARKGRAY : LIGHTGRAY;

                DrawRectangleRec(btnSingle, btnColorSingle);
                DrawRectangleRec(btnInfinito, btnColorInfinito);

                int anchoTxtSingle = MeasureText("Single Round", 20);
                int anchoTxtInfinito = MeasureText("Infinite Rounds", 20);

                DrawText("Single Round", (640 - anchoTxtSingle) / 2, 215, 20, BLACK);
                DrawText("Infinite Rounds", (640 - anchoTxtInfinito) / 2, 295, 20, BLACK);

                DrawText("Use UP/DOWN or W/S to select, ENTER to continue", 130, 350, 15, DARKGRAY);
            } else if (pantallaPersonaje) {
                DrawText("Select Your Character", 200, 100, 20, BLACK);

                float iconSize = 80.0f;
                float startX = 50.0f;
                float startY = 140.0f;
                float spacingX = 180.0f;
                float spacingY = 120.0f;

                struct CharacterSlot {
                    float x, y;
                    const char* name;
                    Texture2D* texture;
                };

                CharacterSlot slots[6] = {
                    {startX, startY, "Pearto", &peartoTexture},
                    {startX + spacingX, startY, "Miku", &mikuTexture},
                    {startX + spacingX * 2, startY, "Neru", &neruTexture},
                    {startX, startY + spacingY, "Luka", &lukaTexture},
                    {startX + spacingX, startY + spacingY, "Kaito", &kaitoTexture},
                    {startX + spacingX * 2, startY + spacingY, "Gumi", &gumiTexture}
                };

                for (int i = 0; i < 6; i++) {
                    Rectangle slotRect = { slots[i].x, slots[i].y, iconSize, iconSize };
                    Color bgColor = (i == personajeSeleccionado) ? DARKGRAY : LIGHTGRAY;
                    DrawRectangleRec(slotRect, bgColor);

                    float scaleWidth = iconSize / slots[i].texture->width;
                    float scaleHeight = iconSize / slots[i].texture->height;
                    float scale = (scaleWidth < scaleHeight) ? scaleWidth : scaleHeight;

                    float scaledWidth = slots[i].texture->width * scale;
                    float scaledHeight = slots[i].texture->height * scale;
                    float offsetX = (iconSize - scaledWidth) / 2.0f;
                    float offsetY = (iconSize - scaledHeight) / 2.0f;

                    Vector2 pos = { slots[i].x + offsetX, slots[i].y + offsetY };
                    DrawTextureEx(*slots[i].texture, pos, 0.0f, scale, WHITE);

                    int nameWidth = MeasureText(slots[i].name, 16);
                    DrawText(slots[i].name, static_cast<int>(slots[i].x + (iconSize - nameWidth) / 2), static_cast<int>(slots[i].y + iconSize + 5), 16, BLACK);
                }

                DrawText("Use ARROWS or WASD to select, ENTER to confirm", 130, 380, 15, DARKGRAY);
            } else if (gameState.getEstadoGameOver()) {
                const char* txtMuerte = "GAME OVER";
                const char* txtReinicio = "Press R o ENTER to retry";

                int anchoTxt1 = MeasureText(txtMuerte, 40);
                int anchoTxt2 = MeasureText(txtReinicio, 20);

                DrawText(txtMuerte, (gameState.getAncho() / 2) - (anchoTxt1 / 2), (gameState.getAlto() / 2) - 40, 40, RED);
                DrawText(txtReinicio, (gameState.getAncho() / 2) - (anchoTxt2 / 2), (gameState.getAlto() / 2) + 10, 20, DARKGRAY);
            } else if (gameState.getEstadoVictoria()) {
                const char* txtVictoria = gameState.getModoInfinito() ? "LEVEL COMPLETE!" : "YOU WIN!";
                const char* txtReinicio = gameState.getModoInfinito() ? "Press R o ENTER for next level" : "Press R o ENTER to play again";

                int anchoTxt1 = MeasureText(txtVictoria, 40);
                int anchoTxt2 = MeasureText(txtReinicio, 20);

                DrawText(txtVictoria, (gameState.getAncho() / 2) - (anchoTxt1 / 2), (gameState.getAlto() / 2) - 40, 40, GREEN);
                DrawText(txtReinicio, (gameState.getAncho() / 2) - (anchoTxt2 / 2), (gameState.getAlto() / 2) + 10, 20, DARKGRAY);

            } else {
                string s = "Asteroids: " + to_string(gameState.getAstsVivos());
                if (gameState.getModoInfinito()) {
                    s += " | Level: " + to_string(gameState.getNivel());
                }
                DrawText(s.c_str(), 10, 10, 20, BLACK);

                if (gameState.getVidasJugador() <= 3) {
                    string textoVidas = "";
                    for(int i = 0; i < gameState.getVidasJugador(); i++) {
                        textoVidas += "<3 ";
                    }
                    int anchoVidas = MeasureText(textoVidas.c_str(), 20);
                    DrawText(textoVidas.c_str(), gameState.getAncho() - anchoVidas - 10, 10, 20, RED);
                } else {
                    string textoVidas = to_string(gameState.getVidasJugador()) + "x ";
                    int anchoNumero = MeasureText(textoVidas.c_str(), 20);
                    int anchoCorazon = MeasureText("<3", 20);
                    int xPos = gameState.getAncho() - anchoNumero - anchoCorazon - 10;
                    DrawText(textoVidas.c_str(), xPos, 10, 20, BLACK);
                    DrawText("<3", xPos + anchoNumero, 10, 20, RED);
                }

                auto& nave = gameState.getNave();

                if (gameState.getTiempoCarga() > 0) {
                    float porcentaje = gameState.getTiempoCarga() / gameState.getTiempoEspecial();
                    if (porcentaje > 1.0){
                        porcentaje = 1.0;
                    }

                    DrawRectangle(nave.getCentro().getX() - 20, nave.getCentro().getY() + 5, 40 * porcentaje, 5, ORANGE);

                    if (porcentaje >= 1.0) {
                        DrawText("Boom!", nave.getCentro().getX() - 35, nave.getCentro().getY() + 15, 15, RED);
                    }
                }

                pintaAsteroides(gameState, tetoTexture);
                pintaDisparos(gameState, baguetteTexture);
                pintaNave(gameState, peartoTexture, mikuTexture, neruTexture, lukaTexture, kaitoTexture, gumiTexture);
            }

            EndDrawing();
        }

        UnloadTexture(tetoTexture);
        UnloadTexture(peartoTexture);
        UnloadTexture(mikuTexture);
        UnloadTexture(neruTexture);
        UnloadTexture(lukaTexture);
        UnloadTexture(kaitoTexture);
        UnloadTexture(gumiTexture);
        UnloadTexture(baguetteTexture);
        CloseWindow();
        return 0;
}