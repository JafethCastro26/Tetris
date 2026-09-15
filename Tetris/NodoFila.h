#ifndef NODO_FILA_H
#define NODO_FILA_H

#include "Tablero.h"

struct NodoFila {
private:
    int celdas[Tablero::COLUMNAS]; 
    NodoFila* siguiente;

    friend class Tablero; // esto es para que nodoFila pueda acceder a los metodos de tablero

public:
    NodoFila() {
        for (int columna = 0; columna < Tablero::COLUMNAS; ++columna) {
            this->celdas[columna] = 0;
        }
        this->siguiente = nullptr;
    }
};

#endif
