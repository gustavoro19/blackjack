#include "Jugador.h"

Jugador::Jugador(string nombre, int fichasIniciales) {
    this->nombre = nombre;
    this->fichas = fichasIniciales;
}

Jugador::~Jugador() {
}

string Jugador::getNombre() {
    return nombre;
}

int Jugador::getFichas() {
    return fichas;
}

bool Jugador::puedePagar(int monto) {
    return monto > 0 && monto <= fichas;
}

// Un jugador juega la ronda solo si aposto (apostar crea su primera mano)
bool Jugador::participa() {
    return !manos.empty();
}

// La apuesta sale de las fichas en el momento de apostar y crea la primera mano.
// Los limites minimo y maximo los valida Juego antes de llamar aqui.
bool Jugador::apostar(int monto) {
    if (!puedePagar(monto)) {
        return false;
    }
    fichas -= monto;
    manos.push_back(Mano(monto));
    return true;
}

bool Jugador::doblar(int indiceMano) {
    Mano& mano = manos[indiceMano];
    if (!mano.puedeDoblar() || !puedePagar(mano.getApuesta())) {
        return false;
    }
    fichas -= mano.getApuesta();
    mano.doblarApuesta();
    return true;
}

// Separa la mano en dos con la misma apuesta. Juego luego reparte una carta a cada una.
bool Jugador::dividir(int indiceMano) {
    if (!manos[indiceMano].puedeDividir() || !puedePagar(manos[indiceMano].getApuesta())) {
        return false;
    }
    int apuesta = manos[indiceMano].getApuesta();
    fichas -= apuesta;

    Mano nuevaMano(apuesta);
    nuevaMano.agregarCarta(manos[indiceMano].quitarSegundaCarta());
    nuevaMano.marcarDividida();
    manos[indiceMano].marcarDividida();
    manos.push_back(nuevaMano);
    return true;
}

// El seguro sale de las fichas del jugador pero se guarda en su mano.
// Puede ser hasta la mitad de la apuesta original.
bool Jugador::colocarSeguro(int monto) {
    if (manos.empty() || monto > manos[0].getApuesta() / 2 || !puedePagar(monto)) {
        return false;
    }
    fichas -= monto;
    manos[0].asegurar(monto);
    return true;
}

void Jugador::cobrar(int monto) {
    fichas += monto;
}

vector<Mano>& Jugador::getManos() {
    return manos;
}

Mano& Jugador::getMano(int indice) {
    return manos[indice];
}

void Jugador::limpiarRonda() {
    manos.clear();
}
