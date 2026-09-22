#include "raylib.h"
#include "Tablero.h"
#include "Pieza.h"
#include "ColaPiezas.h"
#include "PilaHold.h"
#include "ColaEventos.h"
#include "listaHistorial.h"
#include "TablaPuntajes.h"
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <string>

using namespace std;

TipoMovimiento procesarControles(Tablero& tablero, Pieza& pieza,
                                 bool piezaActiva) {
    if (piezaActiva && IsKeyPressed(KEY_LEFT)) {
        Pieza candidata = pieza;
        candidata.mover(0, -1);
        if (tablero.puedeColocar(candidata)) {
            pieza = candidata;
            return MOVER;
        }
    }

    if (piezaActiva && IsKeyPressed(KEY_RIGHT)) {
        Pieza candidata = pieza;
        candidata.mover(0, 1);
        if (tablero.puedeColocar(candidata)) {
            pieza = candidata;
            return MOVER;
        }
    }

    if (piezaActiva && IsKeyPressed(KEY_UP)) {
        Pieza candidata = pieza;
        candidata.rotar();
        if (tablero.puedeColocar(candidata)) {
            pieza = candidata;
            return ROTAR;
        }
    }

    return INICIO;
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

void aplicarEventos(ColaEventos& eventos, float tiempoJuego,
                    float& intervaloNormal, bool& doblePuntaje, int& puntaje) {
    float momento;
    TipoEvento tipo;
    int valor;

    while (eventos.getFrente(momento, tipo, valor) && momento <= tiempoJuego) {
        eventos.extraer(momento, tipo, valor);
        if (tipo == AUMENTAR_DIFICULTAD) {
            intervaloNormal -= 0.15f * valor;
        } else if (tipo == DOBLE_PUNTAJE) {
            doblePuntaje = true;
        } else if (tipo == BONIFICACION) {
            puntaje += valor;
        }
    }
}

TipoMovimiento actualizarCaida(Tablero& tablero, ColaPiezas& cola,
                               Pieza& pieza, bool& piezaActiva,
                               bool& finPartida, float& tiempoCaida,
                               bool& holdUsado, int& puntaje,
                               float intervaloNormal, bool doblePuntaje) {
    TipoMovimiento movimiento = INICIO;
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
            movimiento = BAJAR;
        } else {
            if (tablero.fijarPieza(pieza)) {
                movimiento = COLOCAR;
                holdUsado = false;
                piezaActiva = false;
                int lineasEliminadas = tablero.eliminarFilasCompletas();
                // Cada linea eliminada suma 100 puntos.
                int puntosPorLinea = 100;
                if (doblePuntaje) {
                    puntosPorLinea *= 2;
                }
                puntaje += lineasEliminadas * puntosPorLinea;
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
    return movimiento;
}

void dibujarJuego(Tablero& tablero, ColaPiezas& cola, PilaHold& hold, Pieza& pieza,
                  bool piezaActiva, bool finPartida, int puntaje, bool enPausa,
                  float tiempoJuego, bool enReplay) {
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
    DrawText(TextFormat("Puntaje: %d", puntaje), 355, 80, 22, RAYWHITE);
    DrawText(TextFormat("Tiempo: %.1f s", tiempoJuego), 355, 115, 16, LIGHTGRAY);
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
    if (enReplay) {
        DrawText("REPLAY", 355, 24, 22, SKYBLUE);
        DrawRectangle(355, 500, 100, 35, DARKGRAY);
        DrawText("ANTERIOR", 365, 510, 14, RAYWHITE);
        DrawRectangle(470, 500, 105, 35, DARKGREEN);
        DrawText("SIGUIENTE", 478, 510, 14, RAYWHITE);
        DrawRectangle(355, 550, 220, 35, MAROON);
        DrawText("SALIR DEL REPLAY", 390, 560, 14, RAYWHITE);
    } else if (finPartida) {
        DrawText("FIN DE PARTIDA", 355, 24, 22, RED);
        DrawText("La nueva pieza no cabe", 355, 50, 16, LIGHTGRAY);
        DrawRectangle(355, 75, 220, 40, DARKGREEN);
        DrawText("REPRODUCIR REPLAY", 365, 87, 16, RAYWHITE);
        DrawRectangle(355, 125, 220, 40, DARKGRAY);
        DrawText("MEJORES PUNTAJES", 365, 137, 16, RAYWHITE);
    } else if (enPausa) {
        DrawText("PAUSA", 355, 24, 22, YELLOW);
        DrawText("P: continuar", 355, 50, 16, LIGHTGRAY);
    }
    if (!enReplay) {
        DrawText("ESC: salir", 355, 590, 18, LIGHTGRAY);
        DrawText("C: guardar / intercambiar", 355, 620, 16, LIGHTGRAY);
        DrawText("P: pausar / continuar", 355, 650, 16, LIGHTGRAY);
        DrawText("Z/Y: deshacer / rehacer", 355, 470, 16, LIGHTGRAY);
        DrawText("Flechas: mover izq/der", 355, 560, 16, LIGHTGRAY);
        DrawText("Arriba: rotar", 355, 530, 16, LIGHTGRAY);
        DrawText("Mantener abajo: acelerar", 355, 500, 16, LIGHTGRAY);
    }
    EndDrawing();
}

void dibujarInicio(std::string& nombreJugador) {
    BeginDrawing();
    ClearBackground(Color{13, 18, 29, 255});
    DrawText("TETRIS", 205, 130, 52, SKYBLUE);
    DrawText("Escribe tu nombre", 190, 240, 24, RAYWHITE);
    DrawRectangleLines(160, 290, 300, 45, LIGHTGRAY);
    DrawText(nombreJugador.c_str(), 175, 302, 22, RAYWHITE);
    DrawText("ENTER: comenzar", 220, 390, 18, LIGHTGRAY);
    EndDrawing();
}

void dibujarTablaPuntajes(TablaPuntajes& tabla) {
    BeginDrawing();
    ClearBackground(Color{13, 18, 29, 255});
    DrawText("MEJORES PUNTAJES", 150, 55, 32, SKYBLUE);
    for (int i = 0; i < tabla.getCantidad(); ++i) {
        RegistroPuntaje registro = tabla.getRegistro(i);
        DrawText(TextFormat("%d. %s", i + 1, registro.nombre.c_str()),
                 120, 130 + i * 38, 20, RAYWHITE);
        DrawText(TextFormat("%d", registro.puntaje), 470, 130 + i * 38,
                 20, YELLOW);
    }
    DrawRectangle(200, 570, 220, 40, DARKGRAY);
    DrawText("VOLVER", 275, 582, 18, RAYWHITE);
    EndDrawing();
}

void procesarInicio(std::string& nombreJugador, bool& enInicio) {
    int caracter = GetCharPressed();
    while (caracter > 0) {
        if (caracter >= 32 && caracter <= 125 && nombreJugador.length() < 16) {
            nombreJugador += static_cast<char>(caracter);
        }
        caracter = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && !nombreJugador.empty()) {
        nombreJugador.pop_back();
    }
    if (IsKeyPressed(KEY_ENTER) && !nombreJugador.empty()) {
        enInicio = false;
    }
}

int main() {
    Tablero tablero;
    unsigned int semilla = static_cast<unsigned int>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::srand(semilla);
    ColaPiezas cola;
    PilaHold hold;
    ColaEventos eventos;
    listaHistorial historial;
    TablaPuntajes tablaPuntajes;
    const string archivoPuntajes = "mejores_puntajes.json";
    tablaPuntajes.cargar(archivoPuntajes);
    bool holdUsado = false;
    int puntaje = 0;
    cola.agregarBolsa();
    eventos.insertar(30.0f, AUMENTAR_DIFICULTAD, 1);
    eventos.insertar(60.0f, DOBLE_PUNTAJE, 2);
    eventos.insertar(90.0f, BONIFICACION, 100);

    Pieza::Tipo tipo = Pieza::I;
    Pieza pieza;
    bool piezaActiva = false;
    if (cola.desencolar(tipo)) {
        pieza = Pieza(tipo, 0, 3);
        piezaActiva = tablero.puedeColocar(pieza);
    }
    bool finPartida = !piezaActiva;
    bool enPausa = false;
    float tiempoCaida = 0.0f;
    float tiempoJuego = 0.0f;
    float intervaloNormal = 0.5f;
    bool doblePuntaje = false;
    bool enReplay = false;
    float tiempoReplay = 0.0f;
    bool enInicio = true;
    bool enTabla = false;
    string nombreJugador;
    bool puntajeGuardado = false;
    historial.guardar(INICIO, tablero, cola, hold, pieza, eventos, puntaje,
                      tiempoJuego, piezaActiva, finPartida, enPausa, holdUsado,
                      intervaloNormal, doblePuntaje);
    InitWindow(620, 680, "Tetris - Tablero");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (enInicio) {
            procesarInicio(nombreJugador, enInicio);
        } else if (enTabla && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
                   CheckCollisionPointRec(GetMousePosition(),
                                          Rectangle{200, 570, 220, 40})) {
            enTabla = false;
        } else if (!enTabla && !enReplay && finPartida &&
                   IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
                   CheckCollisionPointRec(GetMousePosition(),
                                          Rectangle{355, 125, 220, 40})) {
            enTabla = true;
        } else if (!enReplay && finPartida && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
            CheckCollisionPointRec(GetMousePosition(), Rectangle{355, 75, 220, 40})) {
            historial.iniciarReplay(tablero, cola, hold, pieza, eventos, puntaje,
                                    tiempoJuego, piezaActiva, finPartida,
                                    enPausa, holdUsado, intervaloNormal,
                                    doblePuntaje);
            enReplay = true;
            tiempoReplay = 0.0f;
        } else if (enReplay && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
                   CheckCollisionPointRec(GetMousePosition(), Rectangle{355, 550, 220, 35})) {
            enReplay = false;
            finPartida = true;
            tiempoReplay = 0.0f;
        } else if (enReplay && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
                   CheckCollisionPointRec(GetMousePosition(), Rectangle{355, 500, 100, 35})) {
            historial.retrocederReplay(tablero, cola, hold, pieza, eventos,
                                       puntaje, tiempoJuego, piezaActiva,
                                       finPartida, enPausa, holdUsado,
                                       intervaloNormal, doblePuntaje);
        } else if (enReplay && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
                   CheckCollisionPointRec(GetMousePosition(), Rectangle{470, 500, 105, 35})) {
            historial.avanzarReplay(tablero, cola, hold, pieza, eventos,
                                    puntaje, tiempoJuego, piezaActiva,
                                    finPartida, enPausa, holdUsado,
                                    intervaloNormal, doblePuntaje);
        } else if (enReplay) {
            tiempoReplay += GetFrameTime();
            if (tiempoReplay >= 0.15f) {
                historial.avanzarReplay(tablero, cola, hold, pieza, eventos,
                                        puntaje, tiempoJuego, piezaActiva,
                                        finPartida, enPausa, holdUsado,
                                        intervaloNormal, doblePuntaje);
                tiempoReplay = 0.0f;
            }
        } else if (!enReplay && !finPartida && IsKeyPressed(KEY_Z)) {
            historial.deshacer(tablero, cola, hold, pieza, eventos, puntaje,
                               tiempoJuego, piezaActiva, finPartida, enPausa,
                               holdUsado, intervaloNormal, doblePuntaje);
            tiempoCaida = 0.0f;
        } else if (!enReplay && !finPartida && IsKeyPressed(KEY_Y)) {
            historial.rehacer(tablero, cola, hold, pieza, eventos, puntaje,
                              tiempoJuego, piezaActiva, finPartida, enPausa,
                              holdUsado, intervaloNormal, doblePuntaje);
            tiempoCaida = 0.0f;
        } else if (!finPartida && IsKeyPressed(KEY_P)) {
            enPausa = !enPausa;
		
            tiempoCaida = 0.0f;
        } else if (!enReplay && !enPausa && !finPartida) {
            tiempoJuego += GetFrameTime();
            aplicarEventos(eventos, tiempoJuego, intervaloNormal,
                           doblePuntaje, puntaje);
            if (piezaActiva && !holdUsado && IsKeyPressed(KEY_C)) {
                piezaActiva = cambiarHold(tablero, cola, hold, pieza);
                finPartida = !piezaActiva;
                holdUsado = true;
                tiempoCaida = 0.0f;
                historial.guardar(HOLD, tablero, cola, hold, pieza, eventos,
                                  puntaje, tiempoJuego, piezaActiva,
                                  finPartida, enPausa, holdUsado,
                                  intervaloNormal, doblePuntaje);
            } else {
                TipoMovimiento movimiento = procesarControles(tablero, pieza,
                                                               piezaActiva);
                if (movimiento != INICIO) {
                    historial.guardar(movimiento, tablero, cola, hold, pieza,
                                      eventos, puntaje, tiempoJuego,
                                      piezaActiva, finPartida, enPausa,
                                      holdUsado, intervaloNormal,
                                      doblePuntaje);
                }
                movimiento = actualizarCaida(tablero, cola, pieza, piezaActiva,
                                             finPartida, tiempoCaida, holdUsado,
                                             puntaje, intervaloNormal,
                                             doblePuntaje);
                if (movimiento != INICIO) {
                    historial.guardar(movimiento, tablero, cola, hold, pieza,
                                      eventos, puntaje, tiempoJuego,
                                      piezaActiva, finPartida, enPausa,
                                      holdUsado, intervaloNormal,
                                      doblePuntaje);
                }
            }
        }
        if (!enReplay && finPartida && !puntajeGuardado) {
            tablaPuntajes.insertar(nombreJugador, puntaje);
            tablaPuntajes.guardar(archivoPuntajes);
            puntajeGuardado = true;
        }
        if (enInicio) {
            dibujarInicio(nombreJugador);
        } else if (enTabla) {
            dibujarTablaPuntajes(tablaPuntajes);
        } else {
            dibujarJuego(tablero, cola, hold, pieza, piezaActiva, finPartida,
                         puntaje, enPausa, tiempoJuego, enReplay);
        }
    }
    CloseWindow();
    return 0;
}
