#ifndef JUGADOR_H
#define JUGADOR_H
#include <string>
#include <vector>
#include "Mano.h"

using namespace std;

class Jugador {
    private:
        string nombre;
        int saldo;
        int seguro;
        vector<Mano> manos;
    public:
        Jugador(string nombre, int saldoInicial);
        ~Jugador();
        string getNombre();
        int getSaldo();
        int getSeguro();
        bool puedePagar(int monto);
        bool participa();
        bool apostar(int monto);
        bool doblar(int indiceMano);
        bool dividir(int indiceMano);
        bool colocarSeguro(int monto);
        void cobrar(int monto);
        vector<Mano>& getManos();
        Mano& getMano(int indice);
        void limpiarRonda();
};
#endif
