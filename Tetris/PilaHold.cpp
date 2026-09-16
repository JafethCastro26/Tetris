#include "PilaHold.h"

PilaHold::PilaHold() {
    this->piezas[0] = Pieza::I;
    this->ocupada = false;
}

bool PilaHold::estaVacia() {
    return !ocupada;
}

bool PilaHold::apilar(Pieza::Tipo tipo) {
    if (ocupada || tipo < Pieza::I || tipo > Pieza::L) {
        return false;
    }
    piezas[0] = tipo;
    ocupada = true;
    return true;
}

bool PilaHold::desapilar(Pieza::Tipo& tipo) {
    if (estaVacia()) {
        return false;
    }
    tipo = piezas[0];
    ocupada = false;
    return true;
}

bool PilaHold::getTope(Pieza::Tipo& tipo) {
    if (estaVacia()) {
        return false;
    }
    tipo = piezas[0];
    return true;
}
