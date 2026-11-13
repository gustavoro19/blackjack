#ifndef CARTA_H
#define CARTA_H
#include <string>

using namespace std;

class Carta {
    private:
        string rango;
        string palo;
        bool bocaArriba;
        bool esRoja();
        string getSimbolo();
        string colorear(string texto);
    public:
        Carta(string rango, string palo);
        ~Carta();
        int getValor();
        bool esAs();
        bool estaBocaArriba();
        void setBocaArriba(bool bocaArriba);
        string toString();
        string dibujarFila(int fila);
};
#endif
