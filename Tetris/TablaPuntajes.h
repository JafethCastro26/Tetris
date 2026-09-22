#ifndef TABLA_PUNTAJES_H
#define TABLA_PUNTAJES_H

#include "RegistroPuntaje.h"

class TablaPuntajes {
private:
    static const int MAXIMO = 10;
    RegistroPuntaje registros[MAXIMO];
    int cantidad;

    void quickSort(int inicio, int fin);

public:
    TablaPuntajes();
    bool cargar(const std::string& nombreArchivo);
    bool insertar(const std::string& nombre, int puntaje);
    bool guardar(const std::string& nombreArchivo);
    int getCantidad();
    RegistroPuntaje getRegistro(int indice);
    void bubbleSort(RegistroPuntaje datos[], int cantidad);
};

#endif
