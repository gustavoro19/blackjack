#include "Carta.h"

Carta::Carta(string rango, string palo) {
    this->rango = rango;
    this->palo = palo;
    this->bocaArriba = true;
}

Carta::~Carta() {
}

// El As vale 11 aqui; la clase Mano lo baja a 1 cuando la mano se pasaria de 21
int Carta::getValor() {
    if (rango == "A") {
        return 11;
    }
    if (rango == "J" || rango == "Q" || rango == "K") {
        return 10;
    }
    return stoi(rango);
}

bool Carta::esAs() {
    return rango == "A";
}

string Carta::getRango() {
    return rango;
}

string Carta::getPalo() {
    return palo;
}

bool Carta::estaBocaArriba() {
    return bocaArriba;
}

void Carta::setBocaArriba(bool bocaArriba) {
    this->bocaArriba = bocaArriba;
}

string Carta::toString() {
    if (!bocaArriba) {
        return "[ ?? ]";
    }
    return "[" + rango + " " + palo + "]";
}
