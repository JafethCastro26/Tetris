#include "listaHistorial.h"

listaHistorial::listaHistorial() {
    this->primero = nullptr;
    this->actual = nullptr;
    this->replayActual = nullptr;
}

void listaHistorial::restaurar(nodoHistorial* estado, Tablero& tablero,
                               ColaPiezas& cola, PilaHold& hold, Pieza& pieza,
                               ColaEventos& eventos, int& puntaje,
                               float& tiempoJuego, bool& piezaActiva,
                               bool& finPartida, bool& enPausa,
                               bool& holdUsado, float& intervaloNormal,
                               bool& doblePuntaje) {
    tablero = estado->tablero;
    cola = estado->cola;
    hold = estado->hold;
    pieza = estado->pieza;
    eventos = estado->eventos;
    puntaje = estado->puntaje;
    tiempoJuego = estado->tiempoJuego;
    intervaloNormal = estado->intervaloNormal;
    doblePuntaje = estado->doblePuntaje;
    piezaActiva = estado->piezaActiva;
    finPartida = estado->finPartida;
    enPausa = estado->enPausa;
    holdUsado = estado->holdUsado;
}

listaHistorial::~listaHistorial() {
    vaciar();
}

void listaHistorial::eliminarDesde(nodoHistorial* inicio) {
    while (inicio != nullptr) {
        nodoHistorial* aBorrar = inicio;
        inicio = inicio->siguiente;
        delete aBorrar;
    }
}

void listaHistorial::guardar(TipoMovimiento tipo, Tablero& tablero,
                        ColaPiezas& cola, PilaHold& hold, Pieza pieza,
                        ColaEventos& eventos, int puntaje, float tiempoJuego,
                        bool piezaActiva, bool finPartida, bool enPausa,
                        bool holdUsado, float intervaloNormal,
                        bool doblePuntaje) {
    nodoHistorial* nuevo = new nodoHistorial(
        tipo, tablero, cola, hold, pieza, eventos, puntaje, tiempoJuego,
        piezaActiva, finPartida, enPausa, holdUsado, intervaloNormal,
        doblePuntaje);

    if (actual == nullptr) {
        primero = nuevo;
    } else {
        eliminarDesde(actual->siguiente);
        actual->siguiente = nuevo;
        nuevo->anterior = actual;
    }
    actual = nuevo;
}

bool listaHistorial::deshacer(Tablero& tablero, ColaPiezas& cola, PilaHold& hold,
                         Pieza& pieza, ColaEventos& eventos, int& puntaje,
                         float& tiempoJuego, bool& piezaActiva,
                         bool& finPartida, bool& enPausa, bool& holdUsado,
                         float& intervaloNormal, bool& doblePuntaje) {
    if (actual == nullptr || actual->anterior == nullptr) {
        return false;
    }

    actual = actual->anterior;
    restaurar(actual, tablero, cola, hold, pieza, eventos, puntaje,
              tiempoJuego, piezaActiva, finPartida, enPausa, holdUsado,
              intervaloNormal, doblePuntaje);
    return true;
}

bool listaHistorial::rehacer(Tablero& tablero, ColaPiezas& cola, PilaHold& hold,
                        Pieza& pieza, ColaEventos& eventos, int& puntaje,
                        float& tiempoJuego, bool& piezaActiva,
                        bool& finPartida, bool& enPausa, bool& holdUsado,
                        float& intervaloNormal, bool& doblePuntaje) {
    if (actual == nullptr || actual->siguiente == nullptr) {
        return false;
    }

    actual = actual->siguiente;
    restaurar(actual, tablero, cola, hold, pieza, eventos, puntaje,
              tiempoJuego, piezaActiva, finPartida, enPausa, holdUsado,
              intervaloNormal, doblePuntaje);
    return true;
}

bool listaHistorial::iniciarReplay(Tablero& tablero, ColaPiezas& cola,
                                   PilaHold& hold, Pieza& pieza,
                                   ColaEventos& eventos, int& puntaje,
                                   float& tiempoJuego, bool& piezaActiva,
                                   bool& finPartida, bool& enPausa,
                                   bool& holdUsado, float& intervaloNormal,
                                   bool& doblePuntaje) {
    if (primero == nullptr) return false;
    replayActual = primero;
    restaurar(replayActual, tablero, cola, hold, pieza, eventos, puntaje,
              tiempoJuego, piezaActiva, finPartida, enPausa, holdUsado,
              intervaloNormal, doblePuntaje);
    return true;
}

bool listaHistorial::avanzarReplay(Tablero& tablero, ColaPiezas& cola,
                                   PilaHold& hold, Pieza& pieza,
                                   ColaEventos& eventos, int& puntaje,
                                   float& tiempoJuego, bool& piezaActiva,
                                   bool& finPartida, bool& enPausa,
                                   bool& holdUsado, float& intervaloNormal,
                                   bool& doblePuntaje) {
    if (replayActual == nullptr || replayActual->siguiente == nullptr) return false;
    replayActual = replayActual->siguiente;
    restaurar(replayActual, tablero, cola, hold, pieza, eventos, puntaje,
              tiempoJuego, piezaActiva, finPartida, enPausa, holdUsado,
              intervaloNormal, doblePuntaje);
    return true;
}

bool listaHistorial::retrocederReplay(Tablero& tablero, ColaPiezas& cola,
                                      PilaHold& hold, Pieza& pieza,
                                      ColaEventos& eventos, int& puntaje,
                                      float& tiempoJuego, bool& piezaActiva,
                                      bool& finPartida, bool& enPausa,
                                      bool& holdUsado, float& intervaloNormal,
                                      bool& doblePuntaje) {
    if (replayActual == nullptr || replayActual->anterior == nullptr) return false;
    replayActual = replayActual->anterior;
    restaurar(replayActual, tablero, cola, hold, pieza, eventos, puntaje,
              tiempoJuego, piezaActiva, finPartida, enPausa, holdUsado,
              intervaloNormal, doblePuntaje);
    return true;
}

void listaHistorial::vaciar() {
    eliminarDesde(primero);
    primero = nullptr;
    actual = nullptr;
}
