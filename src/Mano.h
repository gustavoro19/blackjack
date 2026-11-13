#ifndef MANO_H
#define MANO_H
#include <string>
#include <vector>
#include "Carta.h"

using namespace std;

class Mano {
    private:
        vector<Carta> cartas;
        int apuesta;
        int seguro;
        bool doblada;
        bool dividida;
    public:
        Mano();
        ~Mano();
        Mano(int apuesta);
        void agregarCarta(Carta carta);
        int getValor();
        int getValorVisible();
        bool esBlackjack();
        bool sePaso();
        bool puedeDoblar();
        bool puedeDividir();
        Carta quitarSegundaCarta();
        void doblarApuesta();
        void asegurar(int monto);
        void marcarDividida();
        int getApuesta();
        int getSeguro();
        vector<Carta>& getCartas();
        string toString();
        string dibujar();
};
#endif
