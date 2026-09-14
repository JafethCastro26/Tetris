#ifndef NODO_FILA_H
#define NODO_FILA_H

#include "Tablero.h"

struct NodoFila {
private:
    int celdas[Tablero::COLUMNAS]; // 0: vacia; 1 a 7: tipo de bloque.
    NodoFila* siguiente;

    friend class Tablero;

public:
    NodoFila() : celdas{}, siguiente(nullptr) {}
};

#endif
