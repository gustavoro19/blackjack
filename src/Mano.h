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
        bool plantada;
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
        void plantar();
        bool estaPlantada();
        bool estaDoblada();
        bool esDividida();
        int getApuesta();
        int getSeguro();
        int getCantidadCartas();
        vector<Carta>& getCartas();
        string toString();
};
#endif
