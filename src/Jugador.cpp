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

// apostar() crea la primera mano: sin manos, no juega la ronda
bool Jugador::participa() {
    return !manos.empty();
}

// Descuenta al apostar; minimo y maximo los valida Juego
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

// Juego reparte despues una carta a cada mano
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

// Las fichas salen del jugador, pero el seguro queda en su mano
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
