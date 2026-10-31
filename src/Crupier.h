#ifndef CRUPIER_H
#define CRUPIER_H
#include "Mano.h"

class Crupier {
    private:
        Mano mano;
    public:
        Crupier();
        void recibirCarta(Carta carta);
        Mano& getMano();
        Carta getCartaVisible();
        bool muestraAs();
        bool debeRevisarBlackjack();
        bool debePedirCarta();
        void revelarCartaOculta();
        void limpiarMano();
};
#endif
