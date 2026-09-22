#include "Tablero.h"
#include "NodoFila.h"

Tablero::Tablero() {
        this->primera = nullptr;
        for (int i = 0; i < FILAS; ++i) {
            NodoFila* nueva = new NodoFila;
            nueva->siguiente = this->primera;
            this->primera = nueva;
        }
}

Tablero::Tablero(const Tablero& otro) {
    this->primera = nullptr;
    NodoFila* ultima = nullptr;
    NodoFila* actual = otro.primera;

    while (actual != nullptr) {
        NodoFila* nueva = new NodoFila;
        for (int columna = 0; columna < COLUMNAS; ++columna) {
            nueva->celdas[columna] = actual->celdas[columna];
        }

        if (this->primera == nullptr) {
            this->primera = nueva;
        } else {
            ultima->siguiente = nueva;
        }
        ultima = nueva;
        actual = actual->siguiente;
    }
}

Tablero::~Tablero() {
    liberar();
}

Tablero& Tablero::operator=(const Tablero& otro) {
    if (this == &otro) {
        return *this;
    }

    liberar();
    NodoFila* ultima = nullptr;
    NodoFila* actual = otro.primera;

    while (actual != nullptr) {
        NodoFila* nueva = new NodoFila;
        for (int columna = 0; columna < COLUMNAS; ++columna) {
            nueva->celdas[columna] = actual->celdas[columna];
        }

        if (primera == nullptr) {
            primera = nueva;
        } else {
            ultima->siguiente = nueva;
        }
        ultima = nueva;
        actual = actual->siguiente;
    }

    return *this;
}

void Tablero::liberar() {
    while (primera) {
        NodoFila* aBorrar = primera;
        primera = primera->siguiente;
        delete aBorrar;
    }
}

bool Tablero::posicionValida(int fila, int columna)  {
    return fila >= 0 && fila < FILAS && columna >= 0 && columna < COLUMNAS;
}

bool Tablero::puedeColocar(Pieza pieza) {
    for (int i = 0; i < Pieza::BLOQUES; ++i) {
        nodoBloque bloque = pieza.getBloque(i);
        if (!posicionValida(bloque.fila, bloque.columna)) {
            return false;
        }
        if (getCelda(bloque.fila, bloque.columna) != 0) {
            return false;
        }
    }
    return true;
}

bool Tablero::fijarPieza(Pieza pieza) {
    if (!puedeColocar(pieza)) {
        return false;
    }

    for (int i = 0; i < Pieza::BLOQUES; ++i) {
        nodoBloque bloque = pieza.getBloque(i);
        setCelda(bloque.fila, bloque.columna, pieza.getTipo());
    }
    return true;
}

NodoFila* Tablero::buscarFila(int fila)  {
    NodoFila* actual = primera;
    for (int i = 0; i < fila && actual != nullptr; ++i)
        actual = actual->siguiente;
    return actual;
}

int Tablero::getCelda(int fila, int columna)  {
    if (!posicionValida(fila, columna)) return -1; // -1 si la posicion esta fuera del tablero
    return buscarFila(fila)->celdas[columna];
}

bool Tablero::setCelda(int fila, int columna, int valor) {
    if (!posicionValida(fila, columna) || valor < 0 || valor > 7) return false;
    buscarFila(fila)->celdas[columna] = valor;
    return true;
}

void Tablero::vaciar() {
    for (NodoFila* actual = primera; actual != nullptr; actual = actual->siguiente)
        for (int columna = 0; columna < COLUMNAS; ++columna)
            actual->celdas[columna] = 0;
}

int Tablero::eliminarFilasCompletas() {
    int eliminadas = 0;
    NodoFila* anterior = nullptr;
    NodoFila* actual = primera;

    while (actual != nullptr) {
        bool completa = true;
        for (int columna = 0; columna < COLUMNAS; ++columna)
            if (actual->celdas[columna] == 0) completa = false;

        if (!completa) {
            anterior = actual;
            actual = actual->siguiente;
            continue;
        }

       
        NodoFila* vacia = new NodoFila;
        NodoFila* aBorrar = actual;
        actual = actual->siguiente;
        if (anterior != nullptr){
			anterior->siguiente = actual;
		}
        else{
			primera = actual;
		}
        delete aBorrar;

        vacia->siguiente = primera;
        primera = vacia;
        
        if (anterior == nullptr) anterior = vacia;
        ++eliminadas;
    }
    return eliminadas;
}
