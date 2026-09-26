# Tetris 

Proyecto Estructuras de Datos una version de Tetris desarrollada en C++, enfoncandose
al uso de estructuras de datos lineales

## Biblioteca grafica

El proyecto utiliza raylib

## Requisitos

- Windows
- ZinjaI
- C++ 14 en adelante
- raylib instalada y configurada en ZinjaI.


## Ejecucion

1. Abrir `Tetris/Tetris.zpr` en ZinjaI.
2. Compilar el proyecto

## Controles

- Flecha izquierda/derecha: mover la pieza.
- Flecha arriba: rotar la pieza.
- Mantener flecha abajo: acelerar la caida.
- `C`: guardar o intercambiar la pieza en hold. Solo una vez por pieza.
- `P`: pausar o continuar.
- `Z`: deshacer.
- `Y`: rehacer.
- `ESC`: salir.

Al terminar una partida se puede seleccionar `REPRODUCIR REPLAY` para revisar
los estados guardados con avance automatico o con los botones `ANTERIOR` y
`SIGUIENTE`. Tambien se puede consultar `MEJORES PUNTAJES`.

## Estructuras de datos

- `Tablero`: lista enlazada de 20 filas, con 10 celdas por fila.
- `Pieza`: formas, orientaciones y coordenadas de los cuatro bloques
- `ColaPiezas`: cola con bolsas de siete piezas
- `PilaHold`: pila propia de capacidad uno
- `listaHistorial`: lista doblemente enlazada para deshacer, rehacer y replay
- `ColaEventos`: cola ordenada por momento de ejecucion
- `TablaPuntajes`: almacenamiento de los diez mejores puntajes


## Funcionalidades actuales

- Movimiento, rotacion, caida automatica y caida acelerada
- Colisiones con los limites y bloques fijados
- Fijacion de piezas y limpieza de filas completas
- Hold e indicador de las tres piezas siguientes
- Pausa, puntaje y pantalla de fin de partida
- Eventos de dificultad, doble puntaje y bonificacion
- Deshacer, rehacer y replay mediante estados completos
- Tabla de mejores puntajes




