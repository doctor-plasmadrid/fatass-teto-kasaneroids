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

void moverNaveTeclas(Asteroide& nave, float ancho, int izq1, int izq2, int der1, int der2, bool vivo) {
    if (!vivo) {
        return;
    }

    float velocidad = 7.0;
    float xActual = nave.getCentro().getX();
    float radio = nave.getRadio();

    Punto2D velocidadNave(0.0, 0.0);

    if (IsKeyDown(izq1) || IsKeyDown(izq2)) {
        if (xActual - radio > 0) {
            velocidadNave.setXY(-velocidad, 0.0);
        }
    } else if (IsKeyDown(der1) || IsKeyDown(der2)) {
        if (xActual + radio < ancho) {
            velocidadNave.setXY(velocidad, 0.0);
        }
    }

    nave.setVelocidad(velocidadNave);
    nave.mover();
}

void moverNaveHorizontal(GameState& gameState) {
    moverNaveTeclas(gameState.getNave(), gameState.getAncho(),
                    KEY_LEFT, KEY_A, KEY_RIGHT, KEY_D,
                    gameState.getVidasJugador() > 0);
    moverNaveTeclas(gameState.getNave2(), gameState.getAncho(),
                    KEY_J, KEY_NULL, KEY_L, KEY_NULL,
                    gameState.getVidasJugador2() > 0);
}

void colisionNaveAsteroides(GameState& gameState) {
    auto& campoAsteroides = gameState.getCampoAsteroides();
    int astsVivos = gameState.getAstsVivos();
    int vidas1 = gameState.getVidasJugador();
    int vidas2 = gameState.getVidasJugador2();

    auto revisarNave = [&](Asteroide& nave, int& vidas) {
        if (vidas <= 0) {
            return;
        }
        for (int i = 0; i < astsVivos; i++) {
            if (nave.colision(campoAsteroides[i])) {
                vidas--;

                campoAsteroides[i] = campoAsteroides[astsVivos - 1];
                astsVivos--;
                i--;
            }
        }
    };

    revisarNave(gameState.getNave(), vidas1);
    revisarNave(gameState.getNave2(), vidas2);

    gameState.setAstsVivos(astsVivos);
    gameState.setVidasJugador(vidas1);
    gameState.setVidasJugador2(vidas2);

    if (vidas1 <= 0 && vidas2 <= 0) {
        gameState.setEstadoGameOver(true);
    }
}

void colisionesDisparosLote(std::vector<Bala>& disparos, Bala& especial, GameState& gameState, Sound& golpeSound) {
    auto& campoAsteroides = gameState.getCampoAsteroides();
    int astsVivos = gameState.getAstsVivos();
    bool finGolpe = false;

    for (int i = 0; i < gameState.getNumDisp(); i++) {
        if (disparos[i].isActiva()) {
            for (int j = 0; j < astsVivos; j++) {

                if (disparos[i].colision(campoAsteroides[j]) && !finGolpe) {
                    PlaySound(golpeSound);
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
                PlaySound(golpeSound);
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

void colisionesDisparos(GameState& gameState, Sound& golpeSound) {
    colisionesDisparosLote(gameState.getDisparos(), gameState.getEspecial(), gameState, golpeSound);
    colisionesDisparosLote(gameState.getDisparos2(), gameState.getEspecial2(), gameState, golpeSound);
}

void actualizarEstado(GameState& gameState, Sound& golpeSound){
    moverNaveHorizontal(gameState);
    moverAstsCampo(gameState);
    rotarAstsCampo(gameState);
    colisionAsts(gameState);
    colisionBorde(gameState);

    auto& disparos = gameState.getDisparos();
    for (int i = 0; i < gameState.getNumDisp(); i++) {
        disparos[i].actualizar(0);
    }

    auto& disparos2 = gameState.getDisparos2();
    for (int i = 0; i < gameState.getNumDisp(); i++) {
        disparos2[i].actualizar(0);
    }

    auto& especial = gameState.getEspecial();
    especial.actualizar(0);

    auto& especial2 = gameState.getEspecial2();
    especial2.actualizar(0);

    colisionesDisparos(gameState, golpeSound);
    colisionNaveAsteroides(gameState);
}

void manejarDisparoJugador(GameState& gameState, int tecla1, int tecla2,
                           Asteroide& nave, std::vector<Bala>& disparos,
                           Bala& especial, float& tiempoCarga) {
    float posXNave = nave.getCentro().getX();
    float posYNave = nave.getCentro().getY();
    bool fuegoAbajo = IsKeyDown(tecla1) || (tecla2 != KEY_NULL && IsKeyDown(tecla2));
    bool fuegoSueltas = IsKeyReleased(tecla1) || (tecla2 != KEY_NULL && IsKeyReleased(tecla2));

    if (fuegoAbajo) {
        tiempoCarga += GetFrameTime();

        if (tiempoCarga >= gameState.getTiempoEspecial() && !especial.isActiva()) {
            especial.disparar(posXNave, posYNave);
            tiempoCarga = 0.0;
        }
    } else if (fuegoSueltas) {
        if (tiempoCarga > 0.0 && tiempoCarga < gameState.getTiempoEspecial()) {
            bool disparado = false;
            for (int i = 0; i < gameState.getNumDisp(); i++) {
                if (!disparos[i].isActiva() && !disparado) {
                    disparos[i].disparar(posXNave, posYNave);
                    disparado = true;
                }
            }
        }
        tiempoCarga = 0.0;
    }
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
        float scaledWidth = tetoTexture.width * scale * 1.3f;
        float scaledHeight = tetoTexture.height * scale;

        Rectangle srcTex = { 0, 0, (float)tetoTexture.width, (float)tetoTexture.height };
        Rectangle dstTex = { centro.x - (scaledWidth / 2.0f), centro.y - (radio * 3.0f), scaledWidth, scaledHeight };

        DrawTexturePro(tetoTexture, srcTex, dstTex, { 0, 0 }, 0.0f, WHITE);

        DrawPoly(centro, campoAsteroides[i].getLados(), radio, campoAsteroides[i].getGiro(), maroonAlpha);

        DrawPolyLines(centro, campoAsteroides[i].getLados(), radio, campoAsteroides[i].getGiro(), blackAlpha);
    }
}

void pintaPoolDisparos(std::vector<Bala>& disparos, Bala& especial, Texture2D& baguetteTexture) {
    for (int i = 0; i < (int)disparos.size(); i++) {
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

void pintaDisparos(GameState& gameState, Texture2D& baguetteTexture) {
    pintaPoolDisparos(gameState.getDisparos(), gameState.getEspecial(), baguetteTexture);
    pintaPoolDisparos(gameState.getDisparos2(), gameState.getEspecial2(), baguetteTexture);
}

void pintaNave(Asteroide& nave, int personaje, Texture2D& peartoTexture, Texture2D& tetoCharTexture, Texture2D& drTetoTexture, Texture2D& mikuTexture, Texture2D& neruTexture, Texture2D& lukaTexture, Texture2D& kaitoTexture, Texture2D& gumiTexture, Texture2D& rinTexture, Texture2D& lenTexture, Texture2D& meikoTexture, Texture2D& reiTexture){
    Vector2 centro = { nave.getCentro().getX(), nave.getCentro().getY() };
    float radio = nave.getRadio();

    Texture2D* selectedTexture;
    switch (personaje) {
        case 0: selectedTexture = &tetoCharTexture; break;
        case 1: selectedTexture = &drTetoTexture; break;
        case 2: selectedTexture = &peartoTexture; break;
        case 3: selectedTexture = &mikuTexture; break;
        case 4: selectedTexture = &neruTexture; break;
        case 5: selectedTexture = &lukaTexture; break;
        case 6: selectedTexture = &kaitoTexture; break;
        case 7: selectedTexture = &gumiTexture; break;
        case 8: selectedTexture = &rinTexture; break;
        case 9: selectedTexture = &lenTexture; break;
        case 10: selectedTexture = &meikoTexture; break;
        case 11: selectedTexture = &reiTexture; break;
        default: selectedTexture = &tetoCharTexture; break;
    }

    float targetHeight = radio * 2.0f * 2.5f;
    float scale = targetHeight / selectedTexture->height;
    float scaledWidth = selectedTexture->width * scale;
    float scaledHeight = selectedTexture->height * scale;

    Vector2 texturePos = { centro.x - (scaledWidth / 2.0f), centro.y - scaledHeight };

    DrawTextureEx(*selectedTexture, texturePos, 0.0f, scale, WHITE);
}

void reiniciarJuego(GameState& gameState, Sound& nivelSound) {
    gameState.reiniciar();
    PlaySound(nivelSound);
}

bool otraTeclaPresionada() {
    for (int k = 1; k < 400; k++) {
        if (k != KEY_ENTER && k != KEY_R && IsKeyPressed(k)) {
            return true;
        }
    }
    return false;
}

int main(){
        GameState gameState;
        InitWindow(gameState.getAncho(), gameState.getAlto(), "Fatass Teto Kasaneroids");
        bool pantallaTitulo = true;
        bool pantallaJugadores = false;
        bool pantallaSeleccion = false;
        bool pantallaPersonaje = false;
        int opcionSeleccionada = 0;
        int jugadoresSeleccionados = 1;
        int personajeSeleccionado = 0;
        int jugadorActual = 1;
        SetTargetFPS(60);
        InitAudioDevice();

        Sound nivelSound = LoadSound("assets/sfx/teto.wav");
        Music musica = LoadMusicStream("assets/sfx/fukkireta.wav");

        Texture2D tetoTexture = LoadTexture("assets/images/fatass_teto.png");
        Texture2D peartoTexture = LoadTexture("assets/images/pearto.png");
        Texture2D tetoCharTexture = LoadTexture("assets/images/teto.png");
        Texture2D drTetoTexture = LoadTexture("assets/images/dr_teto.png");
        Texture2D mikuTexture = LoadTexture("assets/images/miku.png");
        Texture2D neruTexture = LoadTexture("assets/images/neru.png");
        Texture2D lukaTexture = LoadTexture("assets/images/luka.png");
        Texture2D kaitoTexture = LoadTexture("assets/images/kaito.png");
        Texture2D gumiTexture = LoadTexture("assets/images/gumi.png");
        Texture2D rinTexture = LoadTexture("assets/images/rin.png");
        Texture2D lenTexture = LoadTexture("assets/images/len.png");
        Texture2D meikoTexture = LoadTexture("assets/images/meiko.png");
        Texture2D reiTexture = LoadTexture("assets/images/rei.png");
        Texture2D baguetteTexture = LoadTexture("assets/images/baguette.png");
        Texture2D backgroundTexture = LoadTexture("assets/images/background.png");

        gameState.getEspecial().disparoEspecial();

        while(!WindowShouldClose()){
            bool enPartida = !pantallaTitulo && !pantallaJugadores && !pantallaSeleccion && !pantallaPersonaje &&
                             !gameState.getEstadoGameOver() && !gameState.getEstadoVictoria();

            if (enPartida) {
                if (!IsMusicStreamPlaying(musica)) {
                    PlayMusicStream(musica);
                }
            } else {
                if (IsMusicStreamPlaying(musica)) {
                    StopMusicStream(musica);
                }
            }
            UpdateMusicStream(musica);

            if(pantallaTitulo){
                if(IsKeyPressed(KEY_ENTER)){
                    pantallaTitulo = false;
                    pantallaJugadores = true;
                }
            } else if (pantallaJugadores) {
                if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
                    jugadoresSeleccionados = 1;
                } else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
                    jugadoresSeleccionados = 2;
                } else if (IsKeyPressed(KEY_ENTER)) {
                    pantallaJugadores = false;
                    pantallaSeleccion = true;
                    jugadorActual = 1;
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
                int personajeP1 = gameState.getPersonajeSeleccionado();
                auto moverSeleccion = [&](int delta) {
                    int idx = personajeSeleccionado;
                    do {
                        idx = (idx + delta + 12) % 12;
                    } while (jugadorActual == 2 && idx == personajeP1);
                    personajeSeleccionado = idx;
                };

                if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
                    moverSeleccion(-1);
                } else if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
                    moverSeleccion(1);
                } else if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
                    moverSeleccion(-4);
                } else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
                    moverSeleccion(4);
                } else if (IsKeyPressed(KEY_ENTER)) {
                    if (jugadorActual == 1) {
                        gameState.setModoInfinito(opcionSeleccionada == 1);
                        gameState.setDosJugadores(jugadoresSeleccionados == 2);
                        gameState.setPersonajeSeleccionado(personajeSeleccionado);
                        if (jugadoresSeleccionados == 2) {
                            jugadorActual = 2;
                            personajeSeleccionado = (personajeSeleccionado + 1) % 12;
                        } else {
                            reiniciarJuego(gameState, nivelSound);
                            pantallaPersonaje = false;
                        }
                    } else {
                        gameState.setPersonajeSeleccionado2(personajeSeleccionado);
                        reiniciarJuego(gameState, nivelSound);
                        pantallaPersonaje = false;
                    }
                }
            } else if (gameState.getEstadoGameOver()) {
                if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_R)) {
                    reiniciarJuego(gameState, nivelSound);
                } else if (otraTeclaPresionada()) {
                    pantallaTitulo = true;
                }
            } else if (gameState.getEstadoVictoria()) {
                if (gameState.getModoInfinito()) {
                    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_R)) {
                        gameState.siguienteNivel();
                        PlaySound(nivelSound);
                        gameState.setEstadoVictoria(false);
                    }
                } else {
                    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_R)) {
                        reiniciarJuego(gameState, nivelSound);
                    } else if (otraTeclaPresionada()) {
                        pantallaTitulo = true;
                    }
                }
            } else {
                if (gameState.getVidasJugador() > 0) {
                    manejarDisparoJugador(gameState, KEY_W, KEY_UP,
                                          gameState.getNave(), gameState.getDisparos(),
                                          gameState.getEspecial(), gameState.getTiempoCarga());
                }
                if (gameState.getVidasJugador2() > 0) {
                    manejarDisparoJugador(gameState, KEY_I, KEY_NULL,
                                          gameState.getNave2(), gameState.getDisparos2(),
                                          gameState.getEspecial2(), gameState.getTiempoCarga2());
                }
                actualizarEstado(gameState, nivelSound);

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
            } else if (pantallaJugadores) {
                DrawText("Select Players", 200, 150, 20, BLACK);

                Rectangle btn1P = { 170, 200, 300, 50 };
                Rectangle btn2P = { 170, 280, 300, 50 };

                Color btnColor1P = jugadoresSeleccionados == 1 ? DARKGRAY : LIGHTGRAY;
                Color btnColor2P = jugadoresSeleccionados == 2 ? DARKGRAY : LIGHTGRAY;

                DrawRectangleRec(btn1P, btnColor1P);
                DrawRectangleRec(btn2P, btnColor2P);

                int anchoTxt1P = MeasureText("1 Player", 20);
                int anchoTxt2P = MeasureText("2 Players", 20);

                DrawText("1 Player", (640 - anchoTxt1P) / 2, 215, 20, BLACK);
                DrawText("2 Players", (640 - anchoTxt2P) / 2, 295, 20, BLACK);

                DrawText("Use UP/DOWN or W/S to select, ENTER to continue", 130, 350, 15, DARKGRAY);
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
                string tituloSel = "Player " + to_string(jugadorActual) + " - Select Your Character";
                DrawText(tituloSel.c_str(), 200, 80, 20, BLACK);

                float iconSize = 80.0f;
                float startX = 20.0f;
                float startY = 110.0f;
                float spacingX = 150.0f;
                float spacingY = 110.0f;

                struct CharacterSlot {
                    float x, y;
                    const char* name;
                    Texture2D* texture;
                };

                CharacterSlot slots[12] = {
                    {startX, startY, "Teto", &tetoCharTexture},
                    {startX + spacingX, startY, "Dr. Teto", &drTetoTexture},
                    {startX + spacingX * 2, startY, "Pearto", &peartoTexture},
                    {startX + spacingX * 3, startY, "Miku", &mikuTexture},
                    {startX, startY + spacingY, "Neru", &neruTexture},
                    {startX + spacingX, startY + spacingY, "Luka", &lukaTexture},
                    {startX + spacingX * 2, startY + spacingY, "Kaito", &kaitoTexture},
                    {startX + spacingX * 3, startY + spacingY, "Gumi", &gumiTexture},
                    {startX, startY + spacingY * 2, "Rin", &rinTexture},
                    {startX + spacingX, startY + spacingY * 2, "Len", &lenTexture},
                    {startX + spacingX * 2, startY + spacingY * 2, "Meiko", &meikoTexture},
                    {startX + spacingX * 3, startY + spacingY * 2, "Rei", &reiTexture}
                };

                for (int i = 0; i < 12; i++) {
                    Rectangle slotRect = { slots[i].x, slots[i].y, iconSize, iconSize };
                    Color bgColor = (i == personajeSeleccionado) ? DARKGRAY : LIGHTGRAY;
                    DrawRectangleRec(slotRect, bgColor);

                    if (jugadorActual == 2 && i == gameState.getPersonajeSeleccionado()) {
                        DrawRectangleLines((int)slots[i].x, (int)slots[i].y, (int)iconSize, (int)iconSize, RED);
                        DrawText("P1", static_cast<int>(slots[i].x + iconSize - 22), static_cast<int>(slots[i].y + 2), 16, RED);
                    }

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

                DrawText("Use ARROWS or WASD to select, ENTER to confirm", 130, 460, 15, DARKGRAY);
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
                Rectangle srcBg = { 0, 0, (float)backgroundTexture.width, (float)backgroundTexture.height };
                Rectangle dstBg = { 0, 0, (float)gameState.getAncho(), (float)gameState.getAlto() };
                DrawTexturePro(backgroundTexture, srcBg, dstBg, { 0, 0 }, 0.0f, WHITE);

                string s = "Asteroids: " + to_string(gameState.getAstsVivos());
                if (gameState.getModoInfinito()) {
                    s += " | Level: " + to_string(gameState.getNivel());
                }
                DrawText(s.c_str(), 10, 10, 20, WHITE);

                auto dibujaVidas = [&](int vidas, int y) {
                    if (vidas <= 3) {
                        string textoVidas = "";
                        for (int i = 0; i < vidas; i++) {
                            textoVidas += "<3 ";
                        }
                        int anchoVidas = MeasureText(textoVidas.c_str(), 20);
                        DrawText(textoVidas.c_str(), gameState.getAncho() - anchoVidas - 10, y, 20, RED);
                    } else {
                        string textoVidas = to_string(vidas) + "x ";
                        int anchoNumero = MeasureText(textoVidas.c_str(), 20);
                        int anchoCorazon = MeasureText("<3", 20);
                        int xPos = gameState.getAncho() - anchoNumero - anchoCorazon - 10;
                        DrawText(textoVidas.c_str(), xPos, y, 20, WHITE);
                        DrawText("<3", xPos + anchoNumero, y, 20, RED);
                    }
                };
                dibujaVidas(gameState.getVidasJugador(), 10);
                dibujaVidas(gameState.getVidasJugador2(), 35);

                auto dibujaCarga = [&](Asteroide& nave, float carga, bool vivo) {
                    if (!vivo || carga <= 0) {
                        return;
                    }
                    float porcentaje = carga / gameState.getTiempoEspecial();
                    if (porcentaje > 1.0) {
                        porcentaje = 1.0;
                    }

                    DrawRectangle(nave.getCentro().getX() - 20, nave.getCentro().getY() + 5, 40 * porcentaje, 5, ORANGE);

                    if (porcentaje >= 1.0) {
                        DrawText("Boom!", nave.getCentro().getX() - 35, nave.getCentro().getY() + 15, 15, RED);
                    }
                };
                dibujaCarga(gameState.getNave(), gameState.getTiempoCarga(), gameState.getVidasJugador() > 0);
                dibujaCarga(gameState.getNave2(), gameState.getTiempoCarga2(), gameState.getVidasJugador2() > 0);

                pintaAsteroides(gameState, tetoTexture);
                pintaDisparos(gameState, baguetteTexture);
                if (gameState.getVidasJugador() > 0) {
                    pintaNave(gameState.getNave(), gameState.getPersonajeSeleccionado(), peartoTexture, tetoCharTexture, drTetoTexture, mikuTexture, neruTexture, lukaTexture, kaitoTexture, gumiTexture, rinTexture, lenTexture, meikoTexture, reiTexture);
                }
                if (gameState.getVidasJugador2() > 0) {
                    pintaNave(gameState.getNave2(), gameState.getPersonajeSeleccionado2(), peartoTexture, tetoCharTexture, drTetoTexture, mikuTexture, neruTexture, lukaTexture, kaitoTexture, gumiTexture, rinTexture, lenTexture, meikoTexture, reiTexture);
                }
            }

            EndDrawing();
        }

        UnloadTexture(tetoTexture);
        UnloadTexture(peartoTexture);
        UnloadTexture(tetoCharTexture);
        UnloadTexture(drTetoTexture);
        UnloadTexture(mikuTexture);
        UnloadTexture(neruTexture);
        UnloadTexture(lukaTexture);
        UnloadTexture(kaitoTexture);
        UnloadTexture(gumiTexture);
        UnloadTexture(rinTexture);
        UnloadTexture(lenTexture);
        UnloadTexture(meikoTexture);
        UnloadTexture(reiTexture);
        UnloadTexture(baguetteTexture);
        UnloadTexture(backgroundTexture);
        UnloadSound(nivelSound);
        UnloadMusicStream(musica);
        CloseAudioDevice();
        CloseWindow();
        return 0;
}