#ifndef COLA_EVENTOS_H
#define COLA_EVENTOS_H

#include "nodoEvento.h"

class ColaEventos {
private:
    nodoEvento* frente;

public:
    ColaEventos();
    ColaEventos(const ColaEventos& otra);
    ~ColaEventos();

    ColaEventos& operator=(const ColaEventos& otra);

    bool estaVacia();
    bool insertar(float momento, TipoEvento tipo, int valor);
    bool getFrente(float& momento, TipoEvento& tipo, int& valor);
    bool extraer(float& momento, TipoEvento& tipo, int& valor);
    void vaciar();
};

#endif
