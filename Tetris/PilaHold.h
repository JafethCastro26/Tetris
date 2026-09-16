#ifndef PILA_HOLD_H
#define PILA_HOLD_H

#include "Pieza.h"

class PilaHold {
private:
    Pieza::Tipo piezas[1];
    bool ocupada;

public:
    PilaHold();

    bool estaVacia();
    bool apilar(Pieza::Tipo tipo);
    bool desapilar(Pieza::Tipo& tipo);
    bool getTope(Pieza::Tipo& tipo);
};

#endif
