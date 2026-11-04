#include "Juego.h"
#include "Constantes.h"
#include "Consola.h"
#include <iostream>

// El zapato se construye y se baraja una unica vez, al crear el juego
Juego::Juego() : zapato(CANTIDAD_MAZOS) {
    zapato.barajar();
    numeroRonda = 0;
}

Juego::~Juego() {
}

void Juego::agregarJugador(string nombre) {
    jugadores.push_back(Jugador(nombre, FICHAS_INICIALES));
}

void Juego::configurarJugadores() {
    int cantidad = leerEntero("  Cantidad de jugadores (1-" + to_string(MAX_JUGADORES) + "): ", 1, MAX_JUGADORES);
    for (int i = 0; i < cantidad; i++) {
        agregarJugador(leerTexto("  Nombre del jugador " + to_string(i + 1) + ": "));
    }
}

// La partida termina si el zapato ya no alcanza para otra ronda (no se rebaraja)
// o si ningun jugador tiene fichas para la apuesta minima.
bool Juego::partidaTerminada() {
    if (!hayCartasParaRonda()) {
        return true;
    }
    for (int i = 0; i < (int)jugadores.size(); i++) {
        if (jugadores[i].getFichas() >= APUESTA_MINIMA) {
            return false;
        }
    }
    return true;
}

// Orden de la ronda segun el enunciado: apuestas, reparto, seguro y revision
// del blackjack del crupier. Turnos y pagos finales se agregan despues.
void Juego::jugarRonda() {
    limpiarRonda();
    pedirApuestas();
    if (!hayApuestas()) {
        cout << "  Nadie aposto en esta ronda." << endl;
        return;
    }
    repartirCartasIniciales();
    mostrarMesa();
    ofrecerSeguro();

    if (crupier.debeRevisarBlackjack()) {
        cout << "  El crupier revisa su carta oculta..." << endl;
        if (crupier.getMano().esBlackjack()) {
            crupier.revelarCartaOculta();
            cout << "  El crupier tiene BLACKJACK. La ronda termina." << endl;
            resolverBlackjackDelCrupier();
            mostrarMesa();
            return;
        }
        cout << "  El crupier no tiene blackjack." << endl;
    }
    // Sin blackjack del crupier el seguro se pierde (ya se desconto de las fichas)
    for (int i = 0; i < (int)jugadores.size(); i++) {
        if (jugadores[i].participa() && jugadores[i].getMano(0).getSeguro() > 0) {
            cout << "  " << jugadores[i].getNombre() << " pierde su seguro de " << jugadores[i].getMano(0).getSeguro() << "." << endl;
        }
    }

    // shortcut: sin turnos ni pagos todavia, se devuelven las apuestas; se reemplaza el martes/miercoles
    for (int i = 0; i < (int)jugadores.size(); i++) {
        if (jugadores[i].participa()) {
            jugadores[i].cobrar(jugadores[i].getMano(0).getApuesta());
        }
    }
    cout << "  (Turnos y pagos pendientes. Se devolvieron las apuestas.)" << endl;
}

// Quien no alcanza la apuesta minima no juega la ronda, pero los demas si
void Juego::pedirApuestas() {
    for (int i = 0; i < (int)jugadores.size(); i++) {
        Jugador& jugador = jugadores[i];
        if (jugador.getFichas() < APUESTA_MINIMA) {
            cout << "  " << jugador.getNombre() << " no tiene fichas para apostar y no juega." << endl;
            continue;
        }
        int maximo = APUESTA_MAXIMA;
        if (jugador.getFichas() < maximo) {
            maximo = jugador.getFichas();
        }
        int apuesta = leerEntero("  Apuesta de " + jugador.getNombre() + " (fichas " + to_string(jugador.getFichas())
                                 + ", " + to_string(APUESTA_MINIMA) + "-" + to_string(maximo) + "): ",
                                 APUESTA_MINIMA, maximo);
        jugador.apostar(apuesta);
    }
}

bool Juego::hayApuestas() {
    for (int i = 0; i < (int)jugadores.size(); i++) {
        if (jugadores[i].participa()) {
            return true;
        }
    }
    return false;
}

// Solo si el crupier muestra un As. Hasta la mitad de la apuesta y sin pasar las fichas; 0 = no asegurar.
void Juego::ofrecerSeguro() {
    if (!crupier.muestraAs()) {
        return;
    }
    cout << "  El crupier muestra un As: se ofrece seguro." << endl;
    for (int i = 0; i < (int)jugadores.size(); i++) {
        Jugador& jugador = jugadores[i];
        if (!jugador.participa()) {
            continue;
        }
        int maximo = jugador.getMano(0).getApuesta() / 2;
        if (jugador.getFichas() < maximo) {
            maximo = jugador.getFichas();
        }
        if (maximo <= 0) {
            cout << "  " << jugador.getNombre() << " no tiene fichas para asegurar." << endl;
            continue;
        }
        int monto = leerEntero("  Seguro de " + jugador.getNombre() + " (0 = no, maximo "
                               + to_string(maximo) + "): ", 0, maximo);
        if (monto > 0) {
            jugador.colocarSeguro(monto);
        }
    }
}

// El crupier tiene blackjack: se paga el seguro y se resuelve la mano de cada jugador.
// Las apuestas y el seguro ya salieron de las fichas, asi que cobrar() devuelve apuesta + ganancia.
void Juego::resolverBlackjackDelCrupier() {
    for (int i = 0; i < (int)jugadores.size(); i++) {
        Jugador& jugador = jugadores[i];
        if (!jugador.participa()) {
            continue;
        }
        // Seguro 2:1: se devuelve lo asegurado mas el doble como ganancia
        if (jugador.getMano(0).getSeguro() > 0) {
            jugador.cobrar(jugador.getMano(0).getSeguro() * 3);
            cout << "  " << jugador.getNombre() << " cobra su seguro: +" << jugador.getMano(0).getSeguro() * 2 << "." << endl;
        }
        Mano& mano = jugador.getMano(0);
        if (mano.esBlackjack()) {
            jugador.cobrar(mano.getApuesta());
            cout << "  " << jugador.getNombre() << " tambien tiene blackjack: empate, recupera "
                 << mano.getApuesta() << "." << endl;
        } else {
            cout << "  " << jugador.getNombre() << " pierde su apuesta de " << mano.getApuesta() << "." << endl;
        }
    }
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
            if (jugadores[i].participa()) {
                jugadores[i].getMano(0).agregarCarta(zapato.repartir(true));
            }
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
            cout << "   (apuesta " << manos[m].getApuesta() << ", fichas " << jugadores[i].getFichas() << ")" << endl;
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
