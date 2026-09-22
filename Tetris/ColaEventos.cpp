#include "ColaEventos.h"

ColaEventos::ColaEventos() {
    this->frente = nullptr;
}

ColaEventos::ColaEventos(const ColaEventos& otra) {
    this->frente = nullptr;
    nodoEvento* actual = otra.frente;
    nodoEvento* ultimo = nullptr;

    while (actual != nullptr) {
        nodoEvento* nuevo = new nodoEvento(actual->momento, actual->tipo,
                                           actual->valor);
        if (frente == nullptr) {
            frente = nuevo;
        } else {
            ultimo->siguiente = nuevo;
        }
        ultimo = nuevo;
        actual = actual->siguiente;
    }
}

ColaEventos::~ColaEventos() {
    vaciar();
}

ColaEventos& ColaEventos::operator=(const ColaEventos& otra) {
    if (this == &otra) {
        return *this;
    }

    vaciar();
    nodoEvento* actual = otra.frente;
    while (actual != nullptr) {
        insertar(actual->momento, actual->tipo, actual->valor);
        actual = actual->siguiente;
    }
    return *this;
}

bool ColaEventos::estaVacia() {
    return frente == nullptr;
}

bool ColaEventos::insertar(float momento, TipoEvento tipo, int valor) {
    if (momento < 0.0f) {
        return false;
    }

    nodoEvento* nuevo = new nodoEvento(momento, tipo, valor);

    if (frente == nullptr || momento < frente->momento) {
        nuevo->siguiente = frente;
        frente = nuevo;
    } else {
        nodoEvento* actual = frente;
        while (actual->siguiente != nullptr &&
               actual->siguiente->momento <= momento) {
            actual = actual->siguiente;
        }
        nuevo->siguiente = actual->siguiente;
        actual->siguiente = nuevo;
    }

    return true;
}

bool ColaEventos::getFrente(float& momento, TipoEvento& tipo, int& valor) {
    if (estaVacia()) {
        return false;
    }

    momento = frente->momento;
    tipo = frente->tipo;
    valor = frente->valor;
    return true;
}

bool ColaEventos::extraer(float& momento, TipoEvento& tipo, int& valor) {
    if (!getFrente(momento, tipo, valor)) {
        return false;
    }

    nodoEvento* aBorrar = frente;
    frente = frente->siguiente;
    delete aBorrar;
    return true;
}

void ColaEventos::vaciar() {
    while (frente != nullptr) {
        nodoEvento* aBorrar = frente;
        frente = frente->siguiente;
        delete aBorrar;
    }
}
