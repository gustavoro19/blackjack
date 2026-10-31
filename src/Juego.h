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
    public:
        Juego();
        void agregarJugador(string nombre);
        bool apuestaValida(Jugador& jugador, int monto);
        bool hayCartasParaRonda();
        void repartirCartasIniciales();
        void mostrarMesa();
        void limpiarRonda();
        Zapato& getZapato();
        Crupier& getCrupier();
        vector<Jugador>& getJugadores();
        int getNumeroRonda();
};
#endif
