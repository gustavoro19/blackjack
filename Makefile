# make        -> genera el ejecutable ./blackjack
# make clean  -> borra lo generado
# Si el enlazado falla en macOS (error "tapi ... unknown architecture"):
#   make CXX="g++ -isysroot $(xcrun --show-sdk-path)"
CXX = g++
SRCS = src/main.cpp src/Consola.cpp src/Juego.cpp src/Jugador.cpp src/Crupier.cpp \
       src/Mano.cpp src/Zapato.cpp src/Carta.cpp
HDRS = src/Constantes.h src/Consola.h src/Juego.h src/Jugador.h src/Crupier.h \
       src/Mano.h src/Zapato.h src/Carta.h

blackjack: $(SRCS) $(HDRS)
	$(CXX) -std=c++17 -Wall -Wextra -g -o blackjack $(SRCS)

clean:
	rm -f blackjack
