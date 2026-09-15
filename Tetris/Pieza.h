#ifndef PIEZA_H
#define PIEZA_H

#include "nodoBloque.h"

class Pieza {
public:
  
    enum Tipo : int;
    static const int BLOQUES = 4;
    static const int ORIENTACIONES = 4;

private:
    Tipo tipo;
    int fila;
    int columna;
    int orientacion;

    
    static const nodoBloque formas[7][ORIENTACIONES][BLOQUES]; // 7 posibles formas, 4 posibles orientaciones, 4 bloques por forma

public:
    // Los valores coinciden con los colores y las celdas del tablero.
    enum Tipo : int { I = 1, O, T, S, Z, J, L };

    Pieza(Tipo tipo = I, int fila = 0, int columna = 3);

    Tipo getTipo();
    int getFila();
    int getColumna();
    int getOrientacion();
    nodoBloque getBloque(int indice);
    void mover(int cambioFila, int cambioColumna);
    void rotar();

};

#endif
