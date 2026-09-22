#ifndef NODO_HISTORIAL_H
#define NODO_HISTORIAL_H

#include "Tablero.h"
#include "ColaPiezas.h"
#include "PilaHold.h"
#include "ColaEventos.h"
#include "Pieza.h"

enum TipoMovimiento {
    INICIO,
    MOVER,
    ROTAR,
    BAJAR,
    COLOCAR,
    HOLD
};

struct nodoHistorial {
private:
    TipoMovimiento tipo;
    Tablero tablero;
    ColaPiezas cola;
    PilaHold hold;
    Pieza pieza;
    ColaEventos eventos;
    int puntaje;
    float tiempoJuego;
    float intervaloNormal;
    bool doblePuntaje;
    bool piezaActiva;
    bool finPartida;
    bool enPausa;
    bool holdUsado;
    nodoHistorial* anterior;
    nodoHistorial* siguiente;

    friend class listaHistorial;

public:
    nodoHistorial(TipoMovimiento tipo, Tablero& tablero, ColaPiezas& cola,
                  PilaHold& hold, Pieza pieza, ColaEventos& eventos,
                  int puntaje, float tiempoJuego, bool piezaActiva,
                  bool finPartida, bool enPausa, bool holdUsado,
                  float intervaloNormal, bool doblePuntaje) {
        this->tipo = tipo;
        this->tablero = tablero;
        this->cola = cola;
        this->hold = hold;
        this->pieza = pieza;
        this->eventos = eventos;
        this->puntaje = puntaje;
        this->tiempoJuego = tiempoJuego;
        this->intervaloNormal = intervaloNormal;
        this->doblePuntaje = doblePuntaje;
        this->piezaActiva = piezaActiva;
        this->finPartida = finPartida;
        this->enPausa = enPausa;
        this->holdUsado = holdUsado;
        this->anterior = nullptr;
        this->siguiente = nullptr;
    }
};

#endif
