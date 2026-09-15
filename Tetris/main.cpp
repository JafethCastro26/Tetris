#include "raylib.h"
#include "Tablero.h"
#include "Pieza.h"

int main() {
    const int tamanoCelda = 28;
    const int origenX = 40;
    const int origenY = 70;
    const Color colores[8] = {
        {24, 31, 46, 255}, SKYBLUE, YELLOW, PURPLE,
        GREEN, RED, BLUE, ORANGE
    };

    Tablero tablero;
    Pieza pieza(Pieza::T, 0, 3);
    bool piezaActiva = true;
    bool finPartida = false;
    float tiempoCaida = 0.0f;
    const float intervaloNormal = 0.5f;
    const float intervaloRapido = 0.05f;
    InitWindow(620, 680, "Tetris - Tablero");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
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

        float intervaloCaida = intervaloNormal;
        if (IsKeyDown(KEY_DOWN)) {
            intervaloCaida = intervaloRapido;
        }

        // Reiniciar al cambiar de velocidad para no acumular bajadas repentinas.
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
                    piezaActiva = false;
                    tablero.eliminarFilasCompletas();
                    // T temporal para probar el ciclo; luego saldra de la cola.
                    pieza = Pieza(Pieza::T, 0, 3);
                    if (tablero.puedeColocar(pieza)) {
                        piezaActiva = true;
                    } else {
                        finPartida = true;
                    }
                }
                tiempoCaida = 0.0f;
                break;
            }
        }

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
        if (finPartida) {
            DrawText("FIN DE PARTIDA", 355, 180, 22, RED);
            DrawText("La nueva pieza no cabe", 355, 215, 16, LIGHTGRAY);
        }
        DrawText("ESC: salir", 355, 590, 18, LIGHTGRAY);
        DrawText("Flechas: mover izq/der", 355, 560, 16, LIGHTGRAY);
        DrawText("Arriba: rotar", 355, 530, 16, LIGHTGRAY);
        DrawText("Mantener abajo: acelerar", 355, 500, 16, LIGHTGRAY);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
