#include <iostream>
#include <string>
#include "Constantes.h"
#include "Consola.h"
#include "Juego.h"

using namespace std;

void mostrarTitulo()
{
    cout << endl;
    cout << "  +=======================================================+" << endl;
    cout << "  |              B L A C K J A C K                        |" << endl;
    cout << "  |              Avance - Fase 1                          |" << endl;
    cout << "  +=======================================================+" << endl;
}

void mostrarMenu()
{
    cout << endl;
    cout << "  [1] Ver el zapato de " << CANTIDAD_MAZOS << " mazos" << endl;
    cout << "  [2] Barajar un zapato de prueba (antes y despues)" << endl;
    cout << "  [3] Repartir una ronda" << endl;
    cout << "  [4] Probar el valor de manos" << endl;
    cout << "  [0] Salir" << endl;
    cout << endl;
}

void mostrarZapato(Zapato &zapato)
{
    string rangos[13] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    cout << endl;
    cout << "  Cartas en el zapato: " << zapato.getCartasRestantes()
         << " (" << CANTIDAD_MAZOS << " mazos x " << CARTAS_POR_MAZO << ")" << endl;
    cout << "  Cantidad por rango:" << endl << "  ";
    for (int i = 0; i < 13; i++)
    {
        cout << rangos[i] << ":" << zapato.contarRango(rangos[i]) << "  ";
    }
    cout << endl;
}

void mostrarPrimerasCartas(Zapato &zapato, int cantidad)
{
    cout << "  ";
    for (int i = 0; i < cantidad; i++)
    {
        cout << zapato.verCarta(i).toString() << " ";
    }
    cout << endl;
}

void probarBarajado()
{
    Zapato prueba(CANTIDAD_MAZOS);
    cout << endl << "  Primeras 6 cartas SIN barajar:" << endl;
    mostrarPrimerasCartas(prueba, 6);
    prueba.barajar();
    cout << endl << "  Primeras 6 cartas despues de barajar:" << endl;
    mostrarPrimerasCartas(prueba, 6);
    cout << endl << "  Siguen siendo " << prueba.getCartasRestantes() << " cartas." << endl;
}

void repartirRonda(Juego &juego)
{
    vector<Jugador> &jugadores = juego.getJugadores();
    if (jugadores.empty())
    {
        int cantidad = leerEntero("  Cantidad de jugadores (1-3): ", 1, 3);
        for (int i = 0; i < cantidad; i++)
        {
            juego.agregarJugador(leerTexto("  Nombre del jugador " + to_string(i + 1) + ": "));
        }
    }

    if (!juego.hayCartasParaRonda())
    {
        cout << "  No quedan suficientes cartas en el zapato. Fin de la partida." << endl;
        return;
    }

    juego.limpiarRonda();
    for (int i = 0; i < (int)jugadores.size(); i++)
    {
        Jugador &jugador = jugadores[i];
        if (jugador.getSaldo() < APUESTA_MINIMA)
        {
            cout << "  " << jugador.getNombre() << " no tiene saldo para apostar." << endl;
            return;
        }
        int maximo = APUESTA_MAXIMA;
        if (jugador.getSaldo() < maximo)
        {
            maximo = jugador.getSaldo();
        }
        int apuesta = leerEntero("  Apuesta de " + jugador.getNombre() + " (saldo " + to_string(jugador.getSaldo())
                                 + ", " + to_string(APUESTA_MINIMA) + "-" + to_string(maximo) + "): ",
                                 APUESTA_MINIMA, maximo);
        jugador.apostar(apuesta);
    }

    juego.repartirCartasIniciales();
    juego.mostrarMesa();

    Crupier &crupier = juego.getCrupier();
    if (crupier.debeRevisarBlackjack())
    {
        cout << "  La carta visible del crupier vale 10 o es As: revisaria su carta oculta." << endl;
    }
    cout << "  Carta oculta del crupier: ";
    crupier.revelarCartaOculta();
    cout << crupier.getMano().toString() << " -> " << crupier.getMano().getValor() << endl;
    // Todavia no hay pagos: se devuelve lo apostado para que el saldo no baje
    for (int i = 0; i < (int)jugadores.size(); i++)
    {
        jugadores[i].cobrar(jugadores[i].getMano(0).getApuesta());
    }
    cout << "  (Turnos y pagos se completan en la Fase 2. Se devolvieron las apuestas.)" << endl;
}

void probarMano(string descripcion, string rangos[], int cantidad)
{
    Mano mano(10);
    for (int i = 0; i < cantidad; i++)
    {
        mano.agregarCarta(Carta(rangos[i], "Picas"));
    }
    cout << "  " << descripcion << endl;
    cout << "     " << mano.toString() << "-> " << mano.getValor();
    if (mano.esBlackjack())
    {
        cout << "  blackjack";
    }
    if (mano.sePaso())
    {
        cout << "  se paso";
    }
    if (mano.puedeDividir())
    {
        cout << "  se puede dividir";
    }
    cout << endl;
}

void probarValoresDeMano()
{
    string caso1[] = {"A", "K"};
    string caso2[] = {"A", "6"};
    string caso3[] = {"A", "6", "9"};
    string caso4[] = {"A", "A", "9"};
    string caso5[] = {"K", "Q", "5"};
    string caso6[] = {"8", "8"};
    string caso7[] = {"K", "10"};
    string caso8[] = {"7", "4", "J"};

    cout << endl;
    probarMano("As con figura:", caso1, 2);
    probarMano("As que vale 11:", caso2, 2);
    probarMano("El As baja a 1 para no pasarse:", caso3, 3);
    probarMano("Dos ases: uno vale 11 y el otro 1:", caso4, 3);
    probarMano("Mano que se pasa de 21:", caso5, 3);
    probarMano("Pareja de 8:", caso6, 2);
    probarMano("K y 10 tienen el mismo valor:", caso7, 2);
    probarMano("21 con tres cartas no es blackjack:", caso8, 3);
}

int main()
{
    Juego juego;
    int opcion;

    do
    {
        limpiarPantalla();
        mostrarTitulo();
        mostrarMenu();
        opcion = leerEntero("  Opcion: ", 0, 4);

        switch (opcion)
        {
        case 1:
            mostrarZapato(juego.getZapato());
            pausar();
            break;
        case 2:
            probarBarajado();
            pausar();
            break;
        case 3:
            repartirRonda(juego);
            pausar();
            break;
        case 4:
            probarValoresDeMano();
            pausar();
            break;
        case 0:
            cout << endl << "  Gracias por jugar!" << endl;
            break;
        }
    } while (opcion != 0);

    return 0;
}
