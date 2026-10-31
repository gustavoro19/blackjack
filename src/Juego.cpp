#include "Juego.h"
#include "Constantes.h"
#include <iostream>

// El zapato se construye y se baraja una unica vez, al crear el juego
Juego::Juego() : zapato(CANTIDAD_MAZOS) {
    zapato.barajar();
    numeroRonda = 0;
}

void Juego::agregarJugador(string nombre) {
    jugadores.push_back(Jugador(nombre, SALDO_INICIAL));
}

bool Juego::apuestaValida(Jugador& jugador, int monto) {
    return monto >= APUESTA_MINIMA && monto <= APUESTA_MAXIMA && jugador.puedePagar(monto);
}

// Peor caso por jugador: dividir y pedir hasta 21 con cartas bajas. Con 10 cartas
// por jugador mas 10 del crupier no deberia quedarse sin cartas a media ronda.
bool Juego::hayCartasParaRonda() {
    return zapato.getCartasRestantes() >= (int)(jugadores.size() + 1) * 10;
}

// Dos cartas boca arriba a cada jugador; el crupier una arriba y otra abajo.
// Se reparte en orden de mesa: primera carta a todos, luego la segunda.
void Juego::repartirCartasIniciales() {
    numeroRonda++;
    for (int vuelta = 0; vuelta < 2; vuelta++) {
        for (int i = 0; i < (int)jugadores.size(); i++) {
            jugadores[i].getMano(0).agregarCarta(zapato.repartir(true));
        }
        bool bocaArriba = (vuelta == 0);
        crupier.recibirCarta(zapato.repartir(bocaArriba));
    }
}

void Juego::mostrarMesa() {
    Mano& manoCrupier = crupier.getMano();
    cout << endl;
    cout << "  RONDA " << numeroRonda << "   (cartas en el zapato: " << zapato.getCartasRestantes() << ")" << endl;
    cout << "  -------------------------------------------------------" << endl;
    cout << "  Crupier: " << manoCrupier.toString() << " -> " << manoCrupier.getValorVisible();
    if (manoCrupier.getValorVisible() != manoCrupier.getValor()) {
        cout << " (visible)";
    }
    cout << endl;

    for (int i = 0; i < (int)jugadores.size(); i++) {
        vector<Mano>& manos = jugadores[i].getManos();
        for (int m = 0; m < (int)manos.size(); m++) {
            cout << "  " << jugadores[i].getNombre() << ": " << manos[m].toString()
                 << " -> " << manos[m].getValor();
            if (manos[m].esBlackjack()) {
                cout << "  BLACKJACK!";
            }
            cout << "   (apuesta " << manos[m].getApuesta() << ", saldo " << jugadores[i].getSaldo() << ")" << endl;
        }
    }
    cout << "  -------------------------------------------------------" << endl;
}

void Juego::limpiarRonda() {
    for (int i = 0; i < (int)jugadores.size(); i++) {
        jugadores[i].limpiarRonda();
    }
    crupier.limpiarMano();
}

Zapato& Juego::getZapato() {
    return zapato;
}

Crupier& Juego::getCrupier() {
    return crupier;
}

vector<Jugador>& Juego::getJugadores() {
    return jugadores;
}

int Juego::getNumeroRonda() {
    return numeroRonda;
}
