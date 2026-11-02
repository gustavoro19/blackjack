#include "Crupier.h"
#include "Constantes.h"

Crupier::Crupier() {
}

Crupier::~Crupier() {
}

void Crupier::recibirCarta(Carta carta) {
    mano.agregarCarta(carta);
}

Mano& Crupier::getMano() {
    return mano;
}

// La primera carta del crupier es la que va boca arriba
Carta Crupier::getCartaVisible() {
    return mano.getCartas()[0];
}

bool Crupier::muestraAs() {
    return getCartaVisible().esAs();
}

// Si la carta visible vale 10 o es un As, el crupier revisa si tiene blackjack
bool Crupier::debeRevisarBlackjack() {
    return getCartaVisible().getValor() >= 10;
}

bool Crupier::debePedirCarta() {
    return mano.getValor() < LIMITE_CRUPIER;
}

void Crupier::revelarCartaOculta() {
    vector<Carta>& cartas = mano.getCartas();
    for (int i = 0; i < (int)cartas.size(); i++) {
        cartas[i].setBocaArriba(true);
    }
}

void Crupier::limpiarMano() {
    mano = Mano();
}
