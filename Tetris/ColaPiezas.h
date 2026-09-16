#ifndef COLA_PIEZAS_H
#define COLA_PIEZAS_H

#include "Pieza.h"

struct NodoPieza;

class ColaPiezas {
private:
    NodoPieza* frente;
    NodoPieza* final;
    int cantidad;

public:
    ColaPiezas();
    ~ColaPiezas();

    bool estaVacia();
    int getCantidad();
    bool encolar(Pieza::Tipo tipo);
    void agregarBolsa();
    bool desencolar(Pieza::Tipo& tipo);
    bool getFrente(Pieza::Tipo& tipo);
    bool getTipoEn(int indice, Pieza::Tipo& tipo);
    void vaciar();
};

#endif
