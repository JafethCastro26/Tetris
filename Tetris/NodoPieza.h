#ifndef NODO_PIEZA_H
#define NODO_PIEZA_H

#include "Pieza.h"

struct NodoPieza {
private:
    Pieza::Tipo tipo;
    NodoPieza* siguiente;

    friend class ColaPiezas; // ColaPiezas puede acceder a los atributos del nodo.

public:
    NodoPieza(Pieza::Tipo tipo) {
        this->tipo = tipo;
        this->siguiente = nullptr;
    }
};

#endif
