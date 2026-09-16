#include "raylib.h"
#include "Tablero.h"
#include "Pieza.h"
#include "ColaPiezas.h"
#include "PilaHold.h"
#include <cstdlib>
#include <ctime>

void procesarControles(Tablero& tablero, Pieza& pieza, bool piezaActiva) {
    if (piezaActiva && IsKeyPressed(KEY_LEFT)) {
        Pieza candidata = pieza;
        candidata.mover(0, -1);
        if (tablero.puedeColocar(candidata)) {
            pieza = candidata;
        }
    }

    if (piezaActiva && IsKeyPressed(KEY_RIGHT)) {
        Pieza candidata = pieza;
        candidata.mover(0, 1);
        if (tablero.puedeColocar(candidata)) {
            pieza = candidata;
        }
    }

    if (piezaActiva && IsKeyPressed(KEY_UP)) {
        Pieza candidata = pieza;
        candidata.rotar();
        if (tablero.puedeColocar(candidata)) {
            pieza = candidata;
        }
    }
}

bool cambiarHold(Tablero& tablero, ColaPiezas& cola, PilaHold& hold, Pieza& pieza) {
    Pieza::Tipo tipoEntrante = Pieza::I;
    if (hold.estaVacia()) {
        if (cola.getCantidad() < 4) {
            cola.agregarBolsa();
        }
        if (!cola.desencolar(tipoEntrante)) {
            return false;
        }
    } else {
        hold.desapilar(tipoEntrante);
    }

    hold.apilar(pieza.getTipo());
    pieza = Pieza(tipoEntrante, 0, 3);
    return tablero.puedeColocar(pieza);
}

void actualizarCaida(Tablero& tablero, ColaPiezas& cola, Pieza& pieza,
                     bool& piezaActiva, bool& finPartida, float& tiempoCaida,
                     bool& holdUsado) {
    const float intervaloNormal = 0.5f;
    const float intervaloRapido = 0.05f;
    float intervaloCaida = intervaloNormal;
    if (IsKeyDown(KEY_DOWN)) {
        intervaloCaida = intervaloRapido;
    }

   
    if (IsKeyPressed(KEY_DOWN) || IsKeyReleased(KEY_DOWN)) {
        tiempoCaida = 0.0f;
    }
    if (piezaActiva) {
        tiempoCaida += GetFrameTime(); // cuanto tiempo duro el ultimo cuadro
    }

    /*
    todo esto se da porque un frame no siempre dura lo mismo
    por lo cual siempre se hacen acumulaciones de duracion
    en cada frame hazta q se alcanze el requerido
    */

    while (piezaActiva && tiempoCaida >= intervaloCaida) { // es cuanto tiempo debe esperar para q se mueva la pieza
        tiempoCaida -= intervaloCaida;
        Pieza candidata = pieza;
        candidata.mover(1, 0);
        if (tablero.puedeColocar(candidata)) {
            pieza = candidata;
        } else {
            if (tablero.fijarPieza(pieza)) {
                holdUsado = false;
                piezaActiva = false;
                tablero.eliminarFilasCompletas();
                // deja al menos tres proximas despues de sacar una pieza.
                if (cola.getCantidad() < 4) {
                    cola.agregarBolsa();
                }
                Pieza::Tipo tipo = Pieza::I;
                if (cola.desencolar(tipo)) {
                    pieza = Pieza(tipo, 0, 3);
                    piezaActiva = tablero.puedeColocar(pieza);
                }
                finPartida = !piezaActiva;
            }
            tiempoCaida = 0.0f;
            break;
        }
    }
}

void dibujarJuego(Tablero& tablero, ColaPiezas& cola, PilaHold& hold, Pieza& pieza,
                  bool piezaActiva, bool finPartida) {
    const int tamanoCelda = 28;
    const int origenX = 40;
    const int origenY = 70;
    const int tamanoVista = 20;
    const int holdX = 375;
    const int holdY = 180;
    const int proximasX = 375;
    const int proximasY = 270;
    const int separacionProximas = 70;
    const Color colores[8] = {
        {24, 31, 46, 255}, SKYBLUE, YELLOW, PURPLE,
        GREEN, RED, BLUE, ORANGE
    };

    BeginDrawing();
    ClearBackground(Color{13, 18, 29, 255});
    DrawText("TETRIS", origenX, 24, 30, RAYWHITE);

    for (int fila = 0; fila < Tablero::FILAS; ++fila) {
        for (int columna = 0; columna < Tablero::COLUMNAS; ++columna) {
            const int x = origenX + columna * tamanoCelda;
            const int y = origenY + fila * tamanoCelda;
            const int valor = tablero.getCelda(fila, columna);
            DrawRectangle(x, y, tamanoCelda - 1, tamanoCelda - 1, colores[valor]);
        }
    }
    // Dibujar la pieza activa sobre las celdas del tablero.
    if (piezaActiva) {
        for (int i = 0; i < Pieza::BLOQUES; ++i) {
            nodoBloque bloque = pieza.getBloque(i);
            int x = origenX + bloque.columna * tamanoCelda;
            int y = origenY + bloque.fila * tamanoCelda;
            DrawRectangle(x, y, tamanoCelda - 1, tamanoCelda - 1,
                          colores[pieza.getTipo()]);
        }
    }

    DrawRectangleLines(origenX - 1, origenY - 1,
                       Tablero::COLUMNAS * tamanoCelda + 1,
                       Tablero::FILAS * tamanoCelda + 1, GRAY);
    DrawText("TABLERO", 355, 80, 22, RAYWHITE);
    DrawText("10 columnas x 20 filas", 355, 115, 16, LIGHTGRAY);
    DrawText("HOLD", 355, 145, 22, RAYWHITE);
    Pieza::Tipo tipoGuardado = Pieza::I;
    if (hold.getTope(tipoGuardado)) {
        Pieza guardada(tipoGuardado, 0, 0);
        for (int i = 0; i < Pieza::BLOQUES; ++i) {
            nodoBloque bloque = guardada.getBloque(i);
            int x = holdX + bloque.columna * tamanoVista;
            int y = holdY + bloque.fila * tamanoVista;
            DrawRectangle(x, y, tamanoVista - 1, tamanoVista - 1,
                          colores[tipoGuardado]);
        }
    } else {
        DrawText("Vacio", holdX, holdY, 16, LIGHTGRAY);
    }
    DrawText("PROXIMAS", 355, 240, 22, RAYWHITE);
    for (int indice = 0; indice < 3; ++indice) {
        Pieza::Tipo tipoProxima = Pieza::I;
        if (cola.getTipoEn(indice, tipoProxima)) {
            // Origen local (0, 0) para dibujar fuera del tablero.
            Pieza proxima(tipoProxima, 0, 0);
            for (int i = 0; i < Pieza::BLOQUES; ++i) {
                nodoBloque bloque = proxima.getBloque(i);
                int x = proximasX + bloque.columna * tamanoVista;
                int y = proximasY + indice * separacionProximas
                        + bloque.fila * tamanoVista;
                DrawRectangle(x, y, tamanoVista - 1, tamanoVista - 1,
                              colores[tipoProxima]);
            }
        }
    }
    if (finPartida) {
        DrawText("FIN DE PARTIDA", 355, 24, 22, RED);
        DrawText("La nueva pieza no cabe", 355, 50, 16, LIGHTGRAY);
    }
    DrawText("ESC: salir", 355, 590, 18, LIGHTGRAY);
    DrawText("C: guardar / intercambiar", 355, 620, 16, LIGHTGRAY);
    DrawText("Flechas: mover izq/der", 355, 560, 16, LIGHTGRAY);
    DrawText("Arriba: rotar", 355, 530, 16, LIGHTGRAY);
    DrawText("Mantener abajo: acelerar", 355, 500, 16, LIGHTGRAY);
    EndDrawing();
}

int main() {
    Tablero tablero;
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    ColaPiezas cola;
    PilaHold hold;
    bool holdUsado = false;
    cola.agregarBolsa();

    Pieza::Tipo tipo = Pieza::I;
    Pieza pieza;
    bool piezaActiva = false;
    if (cola.desencolar(tipo)) {
        pieza = Pieza(tipo, 0, 3);
        piezaActiva = tablero.puedeColocar(pieza);
    }
    bool finPartida = !piezaActiva;
    float tiempoCaida = 0.0f;
    InitWindow(620, 680, "Tetris - Tablero");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (piezaActiva && !holdUsado && IsKeyPressed(KEY_C)) {
            piezaActiva = cambiarHold(tablero, cola, hold, pieza);
            finPartida = !piezaActiva;
            holdUsado = true;
            tiempoCaida = 0.0f;
        } else {
            procesarControles(tablero, pieza, piezaActiva);
            actualizarCaida(tablero, cola, pieza, piezaActiva, finPartida,
                            tiempoCaida, holdUsado);
        }
        dibujarJuego(tablero, cola, hold, pieza, piezaActiva, finPartida);
    }
    CloseWindow();
    return 0;
}
