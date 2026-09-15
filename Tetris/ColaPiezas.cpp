#include "ColaPiezas.h"
#include "NodoPieza.h"
#include <cstdlib>

ColaPiezas::ColaPiezas() {
    this->frente = nullptr;
    this->final = nullptr;
    this->cantidad = 0;
}

ColaPiezas::~ColaPiezas() {
    vaciar();
}

bool ColaPiezas::estaVacia() {
    return frente == nullptr;
}

int ColaPiezas::getCantidad() {
    return cantidad;
}

bool ColaPiezas::encolar(Pieza::Tipo tipo) {
    if (tipo < Pieza::I || tipo > Pieza::L) {
        return false;
    }
    NodoPieza* nuevo = new NodoPieza(tipo);
    if (estaVacia()) {
        frente = nuevo;
    } else {
        final->siguiente = nuevo;
    }
    final = nuevo;
    ++cantidad;
    return true;
}

void ColaPiezas::agregarBolsa() {
    Pieza::Tipo bolsa[7] = {
        Pieza::I, Pieza::O, Pieza::T, Pieza::S,
        Pieza::Z, Pieza::J, Pieza::L
    };

    // el for es decreciente porque la bolsa no puede tener repetidos
    for (int i = 6; i > 0; --i) {
        int elegida = std::rand() % (i + 1);
        Pieza::Tipo temporal = bolsa[i];
        bolsa[i] = bolsa[elegida];
        bolsa[elegida] = temporal;
    }

    for (int i = 0; i < 7; ++i) {
        encolar(bolsa[i]);
    }
}

bool ColaPiezas::desencolar(Pieza::Tipo& tipo) {
    if (estaVacia()) {
        return false;
    }
    NodoPieza* aBorrar = frente;
    tipo = frente->tipo;
    frente = frente->siguiente;
    if (frente == nullptr) {
        final = nullptr;
    }
    delete aBorrar;
    --cantidad;
    return true;
}

bool ColaPiezas::getFrente(Pieza::Tipo& tipo) {
    if (estaVacia()) {
        return false;
    }
    tipo = frente->tipo;
    return true;
}

void ColaPiezas::vaciar() {
    while (frente != nullptr) {
        NodoPieza* aBorrar = frente;
        frente = frente->siguiente;
        delete aBorrar;
    }
    final = nullptr;
    cantidad = 0;
}
