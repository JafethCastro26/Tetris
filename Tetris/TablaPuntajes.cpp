#include "TablaPuntajes.h"
#include "json.hpp"
#include <fstream>
using namespace std;

using json = nlohmann::json;

TablaPuntajes::TablaPuntajes() {
    this->cantidad = 0;
    for (int i = 0; i < MAXIMO; ++i) {
        this->registros[i].nombre = "";
        this->registros[i].puntaje = 0;
    }
}

bool TablaPuntajes::cargar(const std::string& nombreArchivo) {
    ifstream archivo(nombreArchivo.c_str());
    if (!archivo.is_open()) return false;

    try {
        json datos;
        archivo >> datos;
        if (!datos.is_array()) return false;

        cantidad = 0;
        for (const json& dato : datos) {
            if (!dato.is_object() || !dato.contains("nombre") ||
                !dato.contains("puntaje")) continue;
            insertar(dato.at("nombre").get<std::string>(),
                     dato.at("puntaje").get<int>());
        }
    } catch (const json::exception&) {
        cantidad = 0;
        return false;
    }
    return true;
}

bool TablaPuntajes::insertar(const std::string& nombre, int puntaje) {
    if (nombre.empty() || puntaje < 0) return false;
    if (cantidad < MAXIMO) {
        registros[cantidad].nombre = nombre;
        registros[cantidad].puntaje = puntaje;
        ++cantidad;
    } else {
        int menor = 0;
        for (int i = 1; i < MAXIMO; ++i) {
            if (registros[i].puntaje < registros[menor].puntaje) {
                menor = i;
            }
        }
        if (puntaje <= registros[menor].puntaje) return false;
        registros[menor].nombre = nombre;
        registros[menor].puntaje = puntaje;
    }

    quickSort(0, cantidad - 1);
    return true;
}

void TablaPuntajes::quickSort(int inicio, int fin) {
    int izquierda = inicio;
    int derecha = fin;
    int pivote = registros[(inicio + fin) / 2].puntaje;

    while (izquierda <= derecha) {
        while (registros[izquierda].puntaje > pivote) ++izquierda;
        while (registros[derecha].puntaje < pivote) --derecha;
        if (izquierda <= derecha) {
            RegistroPuntaje temporal = registros[izquierda];
            registros[izquierda] = registros[derecha];
            registros[derecha] = temporal;
            ++izquierda;
            --derecha;
        }
    }
    if (inicio < derecha) quickSort(inicio, derecha);
    if (izquierda < fin) quickSort(izquierda, fin);
}

bool TablaPuntajes::guardar(const std::string& nombreArchivo) {
    std::ofstream archivo(nombreArchivo.c_str());
    if (!archivo.is_open()) return false;

    json datos = json::array();
    for (int i = 0; i < cantidad; ++i) {
        datos.push_back({
            {"nombre", registros[i].nombre},
            {"puntaje", registros[i].puntaje}
        });
    }
    archivo << datos.dump(4) << "\n";
    return true;
}

int TablaPuntajes::getCantidad() { return cantidad; }

void TablaPuntajes::bubbleSort(RegistroPuntaje datos[], int cantidad) {
    for (int i = 0; i < cantidad - 1; ++i) {
        for (int j = 0; j < cantidad - i - 1; ++j) {
            if (datos[j].puntaje < datos[j + 1].puntaje) {
                RegistroPuntaje temporal = datos[j];
                datos[j] = datos[j + 1];
                datos[j + 1] = temporal;
            }
        }
    }
}

RegistroPuntaje TablaPuntajes::getRegistro(int indice) {
    RegistroPuntaje vacio;
    vacio.nombre = "";
    vacio.puntaje = 0;
    if (indice < 0 || indice >= cantidad) return vacio;
    return registros[indice];
}
