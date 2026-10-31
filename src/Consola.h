#ifndef CONSOLA_H
#define CONSOLA_H
#include <string>

using namespace std;

void limpiarPantalla();
void pausar();
int leerEntero(const string &mensaje, int minimo, int maximo);
string leerTexto(const string &mensaje);
#endif
