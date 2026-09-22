#ifndef NODO_EVENTO_H
#define NODO_EVENTO_H

enum TipoEvento {
    AUMENTAR_DIFICULTAD,
    DOBLE_PUNTAJE,
    BONIFICACION
};

struct nodoEvento {
private:
    float momento; // frame del juego en el que se ejecuta el evento
    TipoEvento tipo; 
    int valor; // dato auxiliar a el tipo de evento por ejemplo multiplicador de puntaje
    nodoEvento* siguiente;

    friend class ColaEventos;

public:
    nodoEvento(float momento, TipoEvento tipo, int valor) {
        this->momento = momento;
        this->tipo = tipo;
        this->valor = valor;
        this->siguiente = nullptr;
    }
};

#endif
