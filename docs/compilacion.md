# Como compilar

El código está en `src/` y usa C++17. Hacen falta Windows 10 o más nuevo para que se vean los símbolos de los palos y el color rojo.

## Con cl (Developer Command Prompt)

1. Abrir **Developer Command Prompt for VS** (en español: *Símbolo del sistema para desarrolladores de VS*) desde el menú Inicio.

2. Ir a la carpeta del repositorio:

       cd ruta\al\repositorio\blackjack

3. Compilar todos los `.cpp` de `src/`:

       cl /EHsc /std:c++17 src\*.cpp /Fe:blackjack.exe

   Si no hay errores, aparece `blackjack.exe` en la carpeta.

4. Ejecutar:

       blackjack.exe

## Con Visual Studio

1. **Crear un proyecto vacío.** Archivo > Nuevo > Proyecto > *Proyecto vacío* (C++). Ponerle nombre y crearlo.

2. **Agregar el código.** En el Explorador de soluciones, clic derecho en *Archivos de origen* > Agregar > Elemento existente, y seleccionar todos los `.cpp` de `src/`. Repetir en *Archivos de encabezado* con todos los `.h`.

3. **Usar C++17.** Clic derecho en el proyecto > Propiedades > C/C++ > Lenguaje > *Estándar de lenguaje C++* > **ISO C++17**. Aceptar.

4. **Compilar y ejecutar** con **Ctrl+F5** (Iniciar sin depurar).

## Con g++ (macOS o Linux)

    make
    ./blackjack

Si en macOS el enlazado falla con un error de `tapi ... unknown architecture`:

    make CXX="g++ -isysroot $(xcrun --show-sdk-path)"

## Si las cartas no se ven bien

`prepararConsola()` activa UTF-8 y los colores en la consola de Windows. Si salen símbolos raros en vez de ♥♦♣♠, o códigos como `[31m`, la consola es anterior a Windows 10: usar Windows Terminal.
