#include "Zapato.h"
#include <random>

Zapato::Zapato(int cantidadMazos) {
    this->cantidadMazos = cantidadMazos;
    construir();
}

void Zapato::construir() {
    string rangos[13] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    string palos[4] = {"Corazones", "Diamantes", "Treboles", "Picas"};

    cartas.clear();
    for (int m = 0; m < cantidadMazos; m++) {
        for (int p = 0; p < 4; p++) {
            for (int r = 0; r < 13; r++) {
                cartas.push_back(Carta(rangos[r], palos[p]));
            }
        }
    }
}

// Algoritmo de Fisher-Yates: se recorre de atras hacia adelante y cada carta
// se cambia con una posicion al azar entre 0 y ella misma.
void Zapato::barajar() {
    random_device semilla;
    mt19937 generador(semilla());
    for (int i = (int)cartas.size() - 1; i > 0; i--) {
        uniform_int_distribution<int> rango(0, i);
        int j = rango(generador);
        Carta temporal = cartas[i];
        cartas[i] = cartas[j];
        cartas[j] = temporal;
    }
}

// La carta de arriba del zapato es la ultima del vector, asi sacarla es pop_back
Carta Zapato::repartir(bool bocaArriba) {
    Carta carta = cartas.back();
    cartas.pop_back();
    carta.setBocaArriba(bocaArriba);
    return carta;
}

int Zapato::getCartasRestantes() {
    return (int)cartas.size();
}

bool Zapato::estaVacio() {
    return cartas.empty();
}

int Zapato::contarRango(string rango) {
    int cantidad = 0;
    for (int i = 0; i < (int)cartas.size(); i++) {
        if (cartas[i].getRango() == rango) {
            cantidad++;
        }
    }
    return cantidad;
}

// posicion 0 es la carta de arriba (la proxima en salir)
Carta Zapato::verCarta(int posicion) {
    return cartas[cartas.size() - 1 - posicion];
}
