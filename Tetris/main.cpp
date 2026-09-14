#include "raylib.h"
#include "Tablero.h"

int main() {
    const int tamanoCelda = 28;
    const int origenX = 40;
    const int origenY = 70;
    const Color colores[8] = {
        {24, 31, 46, 255}, SKYBLUE, YELLOW, PURPLE,
        GREEN, RED, BLUE, ORANGE
    };

    Tablero tablero;
    InitWindow(620, 680, "Tetris - Tablero");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(Color{13, 18, 29, 255});
        DrawText("TETRIS", origenX, 24, 30, RAYWHITE);

        for (int fila = 0; fila < Tablero::FILAS; ++fila) {
            for (int columna = 0; columna < Tablero::COLUMNAS; ++columna) {
                const int x = origenX + columna * tamanoCelda;
                const int y = origenY + fila * tamanoCelda;
                const int valor = tablero.obtenerCelda(fila, columna);
                DrawRectangle(x, y, tamanoCelda - 1, tamanoCelda - 1, colores[valor]);
            }
        }
        DrawRectangleLines(origenX - 1, origenY - 1,
                           Tablero::COLUMNAS * tamanoCelda + 1,
                           Tablero::FILAS * tamanoCelda + 1, GRAY);
        DrawText("TABLERO", 355, 80, 22, RAYWHITE);
        DrawText("10 columnas x 20 filas", 355, 115, 16, LIGHTGRAY);
        DrawText("ESC: salir", 355, 590, 18, LIGHTGRAY);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
