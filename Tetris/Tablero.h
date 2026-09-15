#ifndef TABLERO_H
#define TABLERO_H

#include "Pieza.h"

struct NodoFila;

class Tablero {
private:
    NodoFila* primera;

public:
    static const int FILAS = 20;
    static const int COLUMNAS = 10;

    Tablero();
    ~Tablero();

    bool posicionValida(int fila, int columna);
    bool puedeColocar(Pieza pieza);
    int getCelda(int fila, int columna);
    bool setCelda(int fila, int columna, int valor);
    int eliminarFilasCompletas();
    void vaciar();
    NodoFila* buscarFila(int fila);
    void liberar();
};

#endif
