#ifndef JUEGO_H
#define JUEGO_H
#include <string>
#include <vector>
#include "Zapato.h"
#include "Crupier.h"
#include "Jugador.h"

using namespace std;

class Juego {
    private:
        Zapato zapato;
        Crupier crupier;
        vector<Jugador> jugadores;
        int numeroRonda;
        void pedirApuestas();
        bool hayApuestas();
        void ofrecerSeguro();
        void resolverBlackjackDelCrupier();
        void turnoJugador(Jugador& jugador);
        void jugarMano(Jugador& jugador, int indiceMano);
        void turnoCrupier();
        int calcularPago(Mano& mano);
        void resolverPagos();
        void mostrarResumen();
    public:
        Juego();
        ~Juego();
        void agregarJugador(string nombre);
        void configurarJugadores();
        void jugar();
        bool hayCartasParaRonda();
        bool partidaTerminada();
        void jugarRonda();
        void repartirCartasIniciales();
        void mostrarMesa();
        void limpiarRonda();
};
#endif
