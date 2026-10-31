# Blackjack en consola

Proyecto de EIF201 Programación I, Universidad Nacional. Es un juego de Blackjack en C++ que se juega en la consola contra el crupier.

Integrante: Gustavo Rojas Porras (402350187)

## Compilar

Con g++:

    make

Con cl (Developer Command Prompt de Visual Studio):

    cl /EHsc /std:c++17 src\*.cpp /Fe:blackjack.exe

Los pasos completos, con Visual Studio también, están en `docs/compilacion.md`.

## Ejecutar

    ./blackjack

En Windows: `blackjack.exe`

Cómo se juega: `docs/juego.md`

## Carpetas

- `src/` código fuente (.cpp y .h)
- `docs/` guías, diagrama UML y documento formal
