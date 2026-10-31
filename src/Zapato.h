#ifndef ZAPATO_H
#define ZAPATO_H
#include <vector>
#include "Carta.h"

using namespace std;

class Zapato {
    private:
        vector<Carta> cartas;
        int cantidadMazos;
    public:
        Zapato(int cantidadMazos);
        void construir();
        void barajar();
        Carta repartir(bool bocaArriba);
        int getCartasRestantes();
        bool estaVacio();
        int contarRango(string rango);
        Carta verCarta(int posicion);
};
#endif
