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

// La primera carta es la visible
Carta Crupier::getCartaVisible() {
    return mano.getCartas()[0];
}

bool Crupier::muestraAs() {
    return getCartaVisible().esAs();
}

// >= 10 cubre 10, J, Q, K y el As (vale 11)
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
