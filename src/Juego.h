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
    public:
        Juego();
        ~Juego();
        void agregarJugador(string nombre);
        void configurarJugadores();
        bool apuestaValida(Jugador& jugador, int monto);
        bool hayCartasParaRonda();
        bool partidaTerminada();
        void jugarRonda();
        void repartirCartasIniciales();
        void mostrarMesa();
        void limpiarRonda();
        Zapato& getZapato();
        Crupier& getCrupier();
        vector<Jugador>& getJugadores();
        int getNumeroRonda();
};
#endif
