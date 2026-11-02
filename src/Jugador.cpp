#include "Jugador.h"

Jugador::Jugador(string nombre, int saldoInicial) {
    this->nombre = nombre;
    this->saldo = saldoInicial;
    this->seguro = 0;
}

Jugador::~Jugador() {
}

string Jugador::getNombre() {
    return nombre;
}

int Jugador::getSaldo() {
    return saldo;
}

int Jugador::getSeguro() {
    return seguro;
}

bool Jugador::puedePagar(int monto) {
    return monto > 0 && monto <= saldo;
}

// Un jugador juega la ronda solo si aposto (apostar crea su primera mano)
bool Jugador::participa() {
    return !manos.empty();
}

// La apuesta sale del saldo en el momento de apostar y crea la primera mano.
// Los limites minimo y maximo los valida Juego antes de llamar aqui.
bool Jugador::apostar(int monto) {
    if (!puedePagar(monto)) {
        return false;
    }
    saldo -= monto;
    manos.push_back(Mano(monto));
    return true;
}

bool Jugador::doblar(int indiceMano) {
    Mano& mano = manos[indiceMano];
    if (!mano.puedeDoblar() || !puedePagar(mano.getApuesta())) {
        return false;
    }
    saldo -= mano.getApuesta();
    mano.doblarApuesta();
    return true;
}

// Separa la mano en dos con la misma apuesta. Juego luego reparte una carta a cada una.
bool Jugador::dividir(int indiceMano) {
    if (!manos[indiceMano].puedeDividir() || !puedePagar(manos[indiceMano].getApuesta())) {
        return false;
    }
    int apuesta = manos[indiceMano].getApuesta();
    saldo -= apuesta;

    Mano nuevaMano(apuesta);
    nuevaMano.agregarCarta(manos[indiceMano].quitarSegundaCarta());
    nuevaMano.marcarDividida();
    manos[indiceMano].marcarDividida();
    manos.push_back(nuevaMano);
    return true;
}

// El seguro puede ser hasta la mitad de la apuesta original
bool Jugador::colocarSeguro(int monto) {
    if (manos.empty() || monto > manos[0].getApuesta() / 2 || !puedePagar(monto)) {
        return false;
    }
    saldo -= monto;
    seguro = monto;
    return true;
}

void Jugador::cobrar(int monto) {
    saldo += monto;
}

vector<Mano>& Jugador::getManos() {
    return manos;
}

Mano& Jugador::getMano(int indice) {
    return manos[indice];
}

void Jugador::limpiarRonda() {
    manos.clear();
    seguro = 0;
}
