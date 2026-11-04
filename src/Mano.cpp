#include "Mano.h"
#include "Constantes.h"

Mano::Mano() {
    this->apuesta = 0;
    this->seguro = 0;
    this->doblada = false;
    this->dividida = false;
    this->plantada = false;
}

Mano::~Mano() {
}

Mano::Mano(int apuesta) {
    this->apuesta = apuesta;
    this->seguro = 0;
    this->doblada = false;
    this->dividida = false;
    this->plantada = false;
}

void Mano::agregarCarta(Carta carta) {
    cartas.push_back(carta);
}

// Todos los ases se cuentan primero como 11. Mientras la mano se pase de 21
// y quede algun As contado como 11, ese As pasa a valer 1 (se restan 10).
int Mano::getValor() {
    int total = 0;
    int asesComoOnce = 0;
    for (int i = 0; i < (int)cartas.size(); i++) {
        total += cartas[i].getValor();
        if (cartas[i].esAs()) {
            asesComoOnce++;
        }
    }
    while (total > BLACKJACK && asesComoOnce > 0) {
        total -= 10;
        asesComoOnce--;
    }
    return total;
}

// Igual que getValor pero ignorando las cartas boca abajo (la oculta del crupier)
int Mano::getValorVisible() {
    Mano visibles;
    for (int i = 0; i < (int)cartas.size(); i++) {
        if (cartas[i].estaBocaArriba()) {
            visibles.agregarCarta(cartas[i]);
        }
    }
    return visibles.getValor();
}

// Un 21 despues de dividir no cuenta como blackjack
bool Mano::esBlackjack() {
    return cartas.size() == 2 && getValor() == BLACKJACK && !dividida;
}

bool Mano::sePaso() {
    return getValor() > BLACKJACK;
}

bool Mano::puedeDoblar() {
    return cartas.size() == 2 && !doblada;
}

// Solo con las dos primeras cartas de la ronda; K y 10 cuentan como mismo valor
bool Mano::puedeDividir() {
    return cartas.size() == 2 && !dividida && cartas[0].getValor() == cartas[1].getValor();
}

Carta Mano::quitarSegundaCarta() {
    Carta carta = cartas[1];
    cartas.pop_back();
    return carta;
}

void Mano::doblarApuesta() {
    apuesta = apuesta * 2;
    doblada = true;
}

void Mano::asegurar(int monto) {
    seguro = monto;
}

void Mano::marcarDividida() {
    dividida = true;
}

void Mano::plantar() {
    plantada = true;
}

bool Mano::estaPlantada() {
    return plantada;
}

bool Mano::estaDoblada() {
    return doblada;
}

bool Mano::esDividida() {
    return dividida;
}

int Mano::getApuesta() {
    return apuesta;
}

int Mano::getSeguro() {
    return seguro;
}

int Mano::getCantidadCartas() {
    return (int)cartas.size();
}

vector<Carta>& Mano::getCartas() {
    return cartas;
}

string Mano::toString() {
    string texto = "";
    for (int i = 0; i < (int)cartas.size(); i++) {
        texto += cartas[i].toString() + " ";
    }
    return texto;
}
