#ifndef NODO_FILA_H
#define NODO_FILA_H

#include "Tablero.h"

struct NodoFila {
private:
    int celdas[Tablero::COLUMNAS]; 
    NodoFila* siguiente;

    friend class Tablero; // esto es para poder acceder a los metodos privados de tablero

public:
    NodoFila() : celdas{}, siguiente(nullptr) {}
};

#endif
