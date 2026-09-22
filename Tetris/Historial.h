#ifndef HISTORIAL_H
#define HISTORIAL_H

#include "nodoHistorial.h"

class Historial {
private:
    nodoHistorial* primero;
    nodoHistorial* actual;

    void eliminarDesde(nodoHistorial* inicio);

public:
    Historial();
    ~Historial();

    void guardar(TipoMovimiento tipo, Tablero& tablero, ColaPiezas& cola,
                 PilaHold& hold, Pieza pieza, ColaEventos& eventos,
                 int puntaje, float tiempoJuego, bool piezaActiva,
                 bool finPartida, bool enPausa, bool holdUsado);
    bool deshacer(Tablero& tablero, ColaPiezas& cola, PilaHold& hold,
                  Pieza& pieza, ColaEventos& eventos, int& puntaje,
                  float& tiempoJuego, bool& piezaActiva, bool& finPartida,
                  bool& enPausa, bool& holdUsado);
    bool rehacer(Tablero& tablero, ColaPiezas& cola, PilaHold& hold,
                 Pieza& pieza, ColaEventos& eventos, int& puntaje,
                 float& tiempoJuego, bool& piezaActiva, bool& finPartida,
                 bool& enPausa, bool& holdUsado);
    void vaciar();
};

#endif
