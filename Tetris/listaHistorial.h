#ifndef LISTA_HISTORIAL_H
#define LISTA_HISTORIAL_H

#include "nodoHistorial.h"

class listaHistorial {
private:
    nodoHistorial* primero;
    nodoHistorial* actual;
    nodoHistorial* replayActual;

    void eliminarDesde(nodoHistorial* inicio);
    void restaurar(nodoHistorial* estado, Tablero& tablero, ColaPiezas& cola,
                   PilaHold& hold, Pieza& pieza, ColaEventos& eventos,
                   int& puntaje, float& tiempoJuego, bool& piezaActiva,
                   bool& finPartida, bool& enPausa, bool& holdUsado,
                   float& intervaloNormal, bool& doblePuntaje);

public:
    listaHistorial();
    ~listaHistorial();

    void guardar(TipoMovimiento tipo, Tablero& tablero, ColaPiezas& cola,
                 PilaHold& hold, Pieza pieza, ColaEventos& eventos,
                 int puntaje, float tiempoJuego, bool piezaActiva,
                 bool finPartida, bool enPausa, bool holdUsado,
                 float intervaloNormal, bool doblePuntaje);
    bool deshacer(Tablero& tablero, ColaPiezas& cola, PilaHold& hold,
                  Pieza& pieza, ColaEventos& eventos, int& puntaje,
                  float& tiempoJuego, bool& piezaActiva, bool& finPartida,
                  bool& enPausa, bool& holdUsado, float& intervaloNormal,
                  bool& doblePuntaje);
    bool rehacer(Tablero& tablero, ColaPiezas& cola, PilaHold& hold,
                 Pieza& pieza, ColaEventos& eventos, int& puntaje,
                 float& tiempoJuego, bool& piezaActiva, bool& finPartida,
                 bool& enPausa, bool& holdUsado, float& intervaloNormal,
                  bool& doblePuntaje);
    bool iniciarReplay(Tablero& tablero, ColaPiezas& cola, PilaHold& hold,
                       Pieza& pieza, ColaEventos& eventos, int& puntaje,
                       float& tiempoJuego, bool& piezaActiva,
                       bool& finPartida, bool& enPausa, bool& holdUsado,
                       float& intervaloNormal, bool& doblePuntaje);
    bool avanzarReplay(Tablero& tablero, ColaPiezas& cola, PilaHold& hold,
                       Pieza& pieza, ColaEventos& eventos, int& puntaje,
                       float& tiempoJuego, bool& piezaActiva,
                       bool& finPartida, bool& enPausa, bool& holdUsado,
                       float& intervaloNormal, bool& doblePuntaje);
    bool retrocederReplay(Tablero& tablero, ColaPiezas& cola, PilaHold& hold,
                          Pieza& pieza, ColaEventos& eventos, int& puntaje,
                          float& tiempoJuego, bool& piezaActiva,
                          bool& finPartida, bool& enPausa, bool& holdUsado,
                          float& intervaloNormal, bool& doblePuntaje);
    void vaciar();
};

#endif
