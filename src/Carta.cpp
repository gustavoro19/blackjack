#include "Carta.h"

// Codigos ANSI de color; los simbolos van en bytes UTF-8 para no depender de la codificacion del archivo
const string ROJO = "\033[31m";
const string NORMAL = "\033[0m";

Carta::Carta(string rango, string palo) {
    this->rango = rango;
    this->palo = palo;
    this->bocaArriba = true;
}

Carta::~Carta() {
}

// El As vale 11 aqui; Mano lo baja a 1 si hace falta
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

bool Carta::estaBocaArriba() {
    return bocaArriba;
}

void Carta::setBocaArriba(bool bocaArriba) {
    this->bocaArriba = bocaArriba;
}

bool Carta::esRoja() {
    return palo == "Corazones" || palo == "Diamantes";
}

string Carta::getSimbolo() {
    if (palo == "Corazones") {
        return "\xE2\x99\xA5";
    }
    if (palo == "Diamantes") {
        return "\xE2\x99\xA6";
    }
    if (palo == "Treboles") {
        return "\xE2\x99\xA3";
    }
    return "\xE2\x99\xA0";
}

// El color se pone despues de rellenar con espacios: los codigos ANSI no ocupan lugar en pantalla
string Carta::colorear(string texto) {
    if (esRoja()) {
        return ROJO + texto + NORMAL;
    }
    return texto;
}

string Carta::toString() {
    if (!bocaArriba) {
        return "??";
    }
    return colorear(rango + getSimbolo());
}

// Fila 0 a 4 del dibujo; cada fila mide 7 columnas
string Carta::dibujarFila(int fila) {
    if (fila == 0 || fila == 4) {
        return "+-----+";
    }
    if (!bocaArriba) {
        return "|#####|";
    }
    string espacios(5 - rango.size(), ' ');
    if (fila == 1) {
        return "|" + colorear(rango) + espacios + "|";
    }
    if (fila == 2) {
        return "|  " + colorear(getSimbolo()) + "  |";
    }
    return "|" + espacios + colorear(rango) + "|";
}
