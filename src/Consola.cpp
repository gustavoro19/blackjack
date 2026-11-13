// windows.h va antes que "using namespace std" (choca con std::byte); NOMINMAX evita su macro max
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif
#include "Consola.h"
#include <iostream>
#include <limits>
#include <cstdlib>

// En Windows: UTF-8 para los simbolos de los palos y codigos ANSI para el color
void prepararConsola()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    HANDLE salida = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD modo = 0;
    GetConsoleMode(salida, &modo);
    SetConsoleMode(salida, modo | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

void limpiarPantalla()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pausar()
{
    cout << endl << "  Presione Enter para continuar...";
    string linea;
    getline(cin, linea);
}

int leerEntero(const string &mensaje, int minimo, int maximo)
{
    int valor;
    while (true)
    {
        cout << mensaje;
        if (cin >> valor)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (valor >= minimo && valor <= maximo)
            {
                return valor;
            }
            cout << "  >> Fuera de rango. Debe ser de " << minimo << " a " << maximo << "." << endl;
        }
        else if (cin.eof())
        {
            cout << endl << "  Entrada terminada. Hasta luego!" << endl;
            exit(0);
        }
        else
        {
            cout << "  >> Entrada invalida. Escribe un numero." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

string leerTexto(const string &mensaje)
{
    string texto;
    while (true)
    {
        cout << mensaje;
        if (!getline(cin, texto))
        {
            cout << endl << "  Entrada terminada. Hasta luego!" << endl;
            exit(0);
        }
        if (!texto.empty())
        {
            return texto;
        }
        cout << "  >> No puede quedar vacio." << endl;
    }
}
