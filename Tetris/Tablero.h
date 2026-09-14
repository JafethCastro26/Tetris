#ifndef TABLERO_H
#define TABLERO_H

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
    int obtenerCelda(int fila, int columna);
    bool establecerCelda(int fila, int columna, int valor);
    int eliminarFilasCompletas();
    void vaciar();
    NodoFila* buscarFila(int fila);
    void liberar();
};

#endif
