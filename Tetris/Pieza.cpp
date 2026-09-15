#include "Pieza.h"

// Orientaciones precalculadas: cada paso gira en sentido horario.
// I utiliza un espacio local de 4x4; las otras, uno de 3x3.
// O conserva sus bloques en las cuatro orientaciones.
const nodoBloque Pieza::formas[7][ORIENTACIONES][BLOQUES] = {
    { // I
        {{1, 0}, {1, 1}, {1, 2}, {1, 3}},
        {{0, 2}, {1, 2}, {2, 2}, {3, 2}},
        {{2, 0}, {2, 1}, {2, 2}, {2, 3}},
        {{0, 1}, {1, 1}, {2, 1}, {3, 1}}
    },
    { // O
        {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
        {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
        {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
        {{0, 1}, {0, 2}, {1, 1}, {1, 2}}
    },
    { // T
        {{0, 1}, {1, 0}, {1, 1}, {1, 2}},
        {{0, 1}, {1, 1}, {1, 2}, {2, 1}},
        {{1, 0}, {1, 1}, {1, 2}, {2, 1}},
        {{0, 1}, {1, 0}, {1, 1}, {2, 1}}
    },
    { // S
        {{0, 1}, {0, 2}, {1, 0}, {1, 1}},
        {{0, 1}, {1, 1}, {1, 2}, {2, 2}},
        {{1, 1}, {1, 2}, {2, 0}, {2, 1}},
        {{0, 0}, {1, 0}, {1, 1}, {2, 1}}
    },
    { // Z
        {{0, 0}, {0, 1}, {1, 1}, {1, 2}},
        {{0, 2}, {1, 1}, {1, 2}, {2, 1}},
        {{1, 0}, {1, 1}, {2, 1}, {2, 2}},
        {{0, 1}, {1, 0}, {1, 1}, {2, 0}}
    },
    { // J
        {{0, 0}, {1, 0}, {1, 1}, {1, 2}},
        {{0, 1}, {0, 2}, {1, 1}, {2, 1}},
        {{1, 0}, {1, 1}, {1, 2}, {2, 2}},
        {{0, 1}, {1, 1}, {2, 0}, {2, 1}}
    },
    { // L
        {{0, 2}, {1, 0}, {1, 1}, {1, 2}},
        {{0, 1}, {1, 1}, {2, 1}, {2, 2}},
        {{1, 0}, {1, 1}, {1, 2}, {2, 0}},
        {{0, 0}, {0, 1}, {1, 1}, {2, 1}}
    }
};

Pieza::Pieza(Tipo tipo, int fila, int columna) {
    if (tipo < I || tipo > L) {
        tipo = I;
    }
    this->tipo = tipo;
    this->fila = fila;
    this->columna = columna;
    this->orientacion = 0;
}

Pieza::Tipo Pieza::getTipo() {
    return tipo;
}

int Pieza::getFila() {
    return fila;
}

int Pieza::getColumna() {
    return columna;
}

int Pieza::getOrientacion() {
    return orientacion;
}

nodoBloque Pieza::getBloque(int indice) {
    if (indice < 0 || indice >= BLOQUES) {
        return {-1, -1}; 
    }
    nodoBloque bloqueEnFila = formas[tipo - 1][orientacion][indice];
    return {fila + bloqueEnFila.fila, columna + bloqueEnFila.columna}; // fila es la de la pieza y la del bloque 
}                                                                      //es la del bloque en la fila, se suman para encontrar la posicion real

void Pieza::mover(int cambioFila, int cambioColumna) {
    fila += cambioFila;
    columna += cambioColumna;
}

void Pieza::rotar() {
    orientacion = (orientacion + 1) % ORIENTACIONES;
	// el residuo es para q despues de la 3 vuelva a la 0
}
