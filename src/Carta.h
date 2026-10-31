#ifndef CARTA_H
#define CARTA_H
#include <string>

using namespace std;

class Carta {
    private:
        string rango;
        string palo;
        bool bocaArriba;
    public:
        Carta(string rango, string palo);
        int getValor();
        bool esAs();
        string getRango();
        string getPalo();
        bool estaBocaArriba();
        void setBocaArriba(bool bocaArriba);
        string toString();
};
#endif
