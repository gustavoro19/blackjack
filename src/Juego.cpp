#include "Juego.h"
#include "Constantes.h"
#include "Consola.h"
#include <iostream>

// Se baraja una sola vez por partida
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

// Termina si no alcanzan las cartas (no se rebaraja) o nadie puede apostar el minimo
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

void Juego::jugar() {
    prepararConsola();
    limpiarPantalla();
    cout << endl;
    cout << "  +=======================================================+" << endl;
    cout << "  |              B L A C K J A C K                        |" << endl;
    cout << "  +=======================================================+" << endl << endl;
    configurarJugadores();
    // Al empezar siempre hay cartas y fichas, por eso se juega antes de preguntar
    while (true) {
        limpiarPantalla();
        jugarRonda();
        if (partidaTerminada()) {
            break;
        }
        if (leerEntero("\n  [1] Siguiente ronda  [0] Terminar partida: ", 0, 1) == 0) {
            break;
        }
    }
    mostrarResumen();
    pausar();
}

void Juego::mostrarResumen() {
    cout << endl << "  ===== FIN DE LA PARTIDA =====" << endl;
    if (!hayCartasParaRonda()) {
        cout << "  No quedan cartas suficientes en el zapato." << endl;
    } else if (partidaTerminada()) {
        cout << "  Ningun jugador tiene fichas para la apuesta minima." << endl;
    }
    cout << "  Rondas jugadas: " << numeroRonda << endl;
    for (int i = 0; i < (int)jugadores.size(); i++) {
        int diferencia = jugadores[i].getFichas() - FICHAS_INICIALES;
        cout << "  " << jugadores[i].getNombre() << ": " << jugadores[i].getFichas() << " fichas (";
        if (diferencia > 0) {
            cout << "gana " << diferencia;
        } else if (diferencia < 0) {
            cout << "pierde " << -diferencia;
        } else {
            cout << "sin cambios";
        }
        cout << ")" << endl;
    }
}

void Juego::jugarRonda() {
    numeroRonda++;
    cout << endl << "  ===== RONDA " << numeroRonda << " =====" << endl;
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
    // Sin blackjack del crupier el seguro se pierde
    for (int i = 0; i < (int)jugadores.size(); i++) {
        if (jugadores[i].participa() && jugadores[i].getMano(0).getSeguro() > 0) {
            cout << "  " << jugadores[i].getNombre() << " pierde su seguro de " << jugadores[i].getMano(0).getSeguro() << "." << endl;
        }
    }

    for (int i = 0; i < (int)jugadores.size(); i++) {
        if (jugadores[i].participa()) {
            turnoJugador(jugadores[i]);
        }
    }

    turnoCrupier();
    resolverPagos();
}

void Juego::turnoCrupier() {
    Mano& mano = crupier.getMano();
    crupier.revelarCartaOculta();
    cout << endl << "  Turno del crupier: " << mano.getValor() << endl << mano.dibujar();

    // Si todas las manos se pasaron o son blackjack, sus cartas no cambian el resultado
    bool hayManoPorComparar = false;
    for (int i = 0; i < (int)jugadores.size(); i++) {
        vector<Mano>& manos = jugadores[i].getManos();
        for (int m = 0; m < (int)manos.size(); m++) {
            if (!manos[m].sePaso() && !manos[m].esBlackjack()) {
                hayManoPorComparar = true;
            }
        }
    }
    if (!hayManoPorComparar) {
        return;
    }

    while (crupier.debePedirCarta()) {
        Carta carta = zapato.repartir(true);
        crupier.recibirCarta(carta);
        cout << "  El crupier pide " << carta.toString() << " -> " << mano.getValor() << endl;
    }
    if (mano.sePaso()) {
        cout << "  El crupier se paso con " << mano.getValor() << "." << endl;
    } else {
        cout << "  El crupier se planta con " << mano.getValor() << "." << endl;
    }
}

// Devuelve lo que recibe el jugador: la apuesta ya salio de sus fichas,
// asi que ganar 1:1 es 2 x apuesta y empatar es la apuesta.
// Aqui el crupier ya no tiene blackjack (ese caso termina antes la ronda).
int Juego::calcularPago(Mano& mano) {
    Mano& manoCrupier = crupier.getMano();
    int apuesta = mano.getApuesta();
    if (mano.sePaso()) {
        return 0;
    }
    if (mano.esBlackjack()) {
        return apuesta + apuesta * 3 / 2;  // division entera: redondea hacia abajo
    }
    if (manoCrupier.sePaso() || mano.getValor() > manoCrupier.getValor()) {
        return apuesta * 2;
    }
    if (mano.getValor() == manoCrupier.getValor()) {
        return apuesta;
    }
    return 0;
}

void Juego::resolverPagos() {
    cout << endl << "  RESULTADOS" << endl;
    for (int i = 0; i < (int)jugadores.size(); i++) {
        Jugador& jugador = jugadores[i];
        vector<Mano>& manos = jugador.getManos();
        for (int m = 0; m < (int)manos.size(); m++) {
            Mano& mano = manos[m];
            int pago = calcularPago(mano);
            jugador.cobrar(pago);

            string nombre = jugador.getNombre();
            if (manos.size() > 1) {
                nombre += " (mano " + to_string(m + 1) + ")";
            }
            cout << "  " << nombre << ": " << mano.getValor() << " -> ";
            if (pago == 0) {
                cout << "pierde " << mano.getApuesta();
            } else if (pago == mano.getApuesta()) {
                cout << "empate, recupera " << pago;
            } else if (mano.esBlackjack()) {
                cout << "BLACKJACK, gana " << pago - mano.getApuesta();
            } else {
                cout << "gana " << pago - mano.getApuesta();
            }
            cout << endl;
        }
        if (jugador.participa()) {
            cout << "  " << jugador.getNombre() << " queda con " << jugador.getFichas() << " fichas." << endl;
        }
    }
}

// while: al dividir aparece una mano nueva que tambien se juega
void Juego::turnoJugador(Jugador& jugador) {
    int indice = 0;
    while (indice < (int)jugador.getManos().size()) {
        jugarMano(jugador, indice);
        indice++;
    }
}

// Los numeros de las acciones son fijos; elegir una no disponible se rechaza
void Juego::jugarMano(Jugador& jugador, int indiceMano) {
    string nombre = jugador.getNombre();
    if (jugador.getManos().size() > 1) {
        nombre += " (mano " + to_string(indiceMano + 1) + ")";
    }

    // El blackjack natural no juega; espera al crupier
    if (jugador.getMano(indiceMano).esBlackjack()) {
        cout << "  " << nombre << " tiene BLACKJACK." << endl;
        return;
    }

    while (true) {
        // Se pide de nuevo: dividir hace push_back y puede mover las manos en memoria
        Mano& mano = jugador.getMano(indiceMano);
        if (mano.sePaso()) {
            cout << "  " << nombre << " se paso con " << mano.getValor() << "." << endl;
            return;
        }
        if (mano.getValor() == BLACKJACK) {
            cout << "  " << nombre << " llega a 21 y se planta." << endl;
            return;
        }

        bool puedeDoblar = mano.puedeDoblar() && jugador.puedePagar(mano.getApuesta());
        // Solo se divide la mano original, una vez
        bool puedeDividir = jugador.getManos().size() == 1 && mano.puedeDividir()
                            && jugador.puedePagar(mano.getApuesta());

        cout << endl << "  Turno de " << nombre << ": " << mano.getValor()
             << "   (apuesta " << mano.getApuesta() << ", fichas " << jugador.getFichas() << ")" << endl
             << mano.dibujar();
        cout << "  Crupier muestra: " << crupier.getCartaVisible().toString() << endl;
        cout << "  [1] Carta  [2] Plantarse";
        if (puedeDoblar) {
            cout << "  [3] Doblar";
        }
        if (puedeDividir) {
            cout << "  [4] Dividir";
        }
        cout << endl;

        int opcion = leerEntero("  Accion: ", 1, 4);
        if (opcion == 1) {
            Carta carta = zapato.repartir(true);
            cout << "  Recibe " << carta.toString() << endl;
            mano.agregarCarta(carta);
        } else if (opcion == 2) {
            cout << "  " << nombre << " se planta con " << mano.getValor() << "." << endl;
            return;
        } else if (opcion == 3 && puedeDoblar) {
            jugador.doblar(indiceMano);
            Mano& doblada = jugador.getMano(indiceMano);
            Carta carta = zapato.repartir(true);
            doblada.agregarCarta(carta);
            cout << "  Dobla a " << doblada.getApuesta() << ", recibe " << carta.toString()
                 << " y queda con " << doblada.getValor() << "." << endl;
            if (doblada.sePaso()) {
                cout << "  " << nombre << " se paso." << endl;
            }
            return;
        } else if (opcion == 4 && puedeDividir) {
            jugador.dividir(indiceMano);
            // Una carta a cada mano; la nueva se juega luego en turnoJugador
            jugador.getMano(indiceMano).agregarCarta(zapato.repartir(true));
            jugador.getMano(jugador.getManos().size() - 1).agregarCarta(zapato.repartir(true));
            cout << "  " << jugador.getNombre() << " divide su mano en dos." << endl;
            nombre = jugador.getNombre() + " (mano " + to_string(indiceMano + 1) + ")";
            cout << "  Mano 1: " << jugador.getMano(indiceMano).toString()
                 << "  Mano 2: " << jugador.getMano(jugador.getManos().size() - 1).toString() << endl;
        } else {
            cout << "  >> Esa accion no esta disponible." << endl;
        }
    }
}

// Quien no alcanza el minimo se salta; los demas juegan
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

// Hasta la mitad de la apuesta; 0 = no asegurar
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

// La apuesta y el seguro ya salieron de las fichas: cobrar() devuelve monto + ganancia
void Juego::resolverBlackjackDelCrupier() {
    for (int i = 0; i < (int)jugadores.size(); i++) {
        Jugador& jugador = jugadores[i];
        if (!jugador.participa()) {
            continue;
        }
        // 2:1 = seguro devuelto + 2 x seguro
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

// Margen de 10 cartas por persona para no quedarse sin cartas a media ronda
bool Juego::hayCartasParaRonda() {
    return zapato.getCartasRestantes() >= (int)(jugadores.size() + 1) * 10;
}

// Orden de mesa: primera carta a todos, luego la segunda (la del crupier boca abajo)
void Juego::repartirCartasIniciales() {
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
    cout << "  Crupier: " << manoCrupier.getValorVisible();
    if (manoCrupier.getValorVisible() != manoCrupier.getValor()) {
        cout << " (visible)";
    }
    cout << endl << manoCrupier.dibujar();

    for (int i = 0; i < (int)jugadores.size(); i++) {
        vector<Mano>& manos = jugadores[i].getManos();
        for (int m = 0; m < (int)manos.size(); m++) {
            cout << "  " << jugadores[i].getNombre() << ": " << manos[m].getValor();
            if (manos[m].esBlackjack()) {
                cout << "  BLACKJACK!";
            }
            cout << "   (apuesta " << manos[m].getApuesta() << ", fichas " << jugadores[i].getFichas() << ")" << endl
                 << manos[m].dibujar();
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
