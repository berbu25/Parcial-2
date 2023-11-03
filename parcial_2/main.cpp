#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>

using namespace std;

class Tablero;

class Jugador {
public:
    enum class Color { blanco, negro };
    Color color;

    Jugador(Color c) : color(c) {}

    void hacer_jugada(Tablero& tablero, int fila, int columna);

    Color obtener_color() const {
            return color;
        }
};

class Casilla {
public:
    enum class Estado { vacia, blanca, negra };
    Estado estado;

    Casilla() : estado(Estado::vacia) {}

    void establecer_estado(Estado nuevo_estado) {
        estado = nuevo_estado;
    }

    Estado obtener_estado() const {
        return estado;
    }

    bool esta_vacia() const {
        return estado == Estado::vacia;
    }

    bool es_blanca() const {
        return estado == Estado::blanca;
    }

    bool es_negra() const {
        return estado == Estado::negra;
    }

    bool es_del_oponente(Jugador::Color color) const;  // Declaración de la función
    bool es_del_jugador(Jugador::Color color) const;  // Declaración de la función

};

class Tablero {
public:
    static const int tamaño = 8;
    vector<vector<Casilla>> tablero;

    Tablero() {
        tablero.resize(tamaño, vector<Casilla>(tamaño));
    }

    void inicializar_tablero() {
        int centro = tamaño / 2;
        tablero[centro - 1][centro - 1].estado = Casilla::Estado::blanca;
        tablero[centro][centro].estado = Casilla::Estado::blanca;
        tablero[centro - 1][centro].estado = Casilla::Estado::negra;
        tablero[centro][centro - 1].estado = Casilla::Estado::negra;
    }

    void mostrar_tablero() {
        for (int i = 0; i < tamaño; i++) {
            for (int j = 0; j < tamaño; j++) {
                if (tablero[i][j].estado == Casilla::Estado::blanca) {
                    cout << "* ";
                } else if (tablero[i][j].estado == Casilla::Estado::negra) {
                    cout << "- ";
                } else {
                    cout << ". ";
                }
            }
            cout << endl;
        }
    }

    bool hacer_jugada(int fila, int columna, Jugador jugador) {
        if (fila < 0 || fila >= tamaño || columna < 0 || columna >= tamaño || !tablero[fila][columna].esta_vacia()) {
            return false;
        }

        int direcciones[8][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

        bool jugada_valida = false;

        for (const auto& direccion : direcciones) {
            int fila_actual = fila + direccion[0];
            int columna_actual = columna + direccion[1];
            bool fichas_a_cambiar = false;

            while (fila_actual >= 0 && fila_actual < tamaño && columna_actual >= 0 && columna_actual < tamaño) {
                Casilla& casilla_actual = tablero[fila_actual][columna_actual];

                if (casilla_actual.esta_vacia()) {
                    break;
                } else if (casilla_actual.es_del_oponente(jugador.color)) {
                    fichas_a_cambiar = true;
                } else if (casilla_actual.es_del_jugador(jugador.color)) {
                    if (fichas_a_cambiar) {
                        jugada_valida = true;

                        while (fila_actual != fila || columna_actual != columna) {
                            fila_actual -= direccion[0];
                            columna_actual -= direccion[1];
                            tablero[fila_actual][columna_actual].establecer_estado(Casilla::Estado::blanca);

                        }
                    }
                    break;
                }

                fila_actual += direccion[0];
                columna_actual += direccion[1];
            }
        }

        if (jugada_valida) {
            tablero[fila][columna].establecer_estado(Casilla::Estado::blanca);
            return true;
        } else {
            return false;
        }
    }

    bool jugador_puede_jugar(Jugador jugador) {
        for (int fila = 0; fila < tamaño; fila++) {
            for (int columna = 0; columna < tamaño; columna++) {
                if (hacer_jugada(fila, columna, jugador)) {
                    return true;
                }
            }
        }
        return false;
    }

    void determinar_ganador(Jugador& jugadorBlanco, Jugador& jugadorNegro) {
        int contadorBlanco = 0;
        int contadorNegro = 0;

        for (int fila = 0; fila < tamaño; fila++) {
            for (int columna = 0; columna < tamaño; columna++) {
                if (tablero[fila][columna].es_blanca()) {
                    contadorBlanco++;
                } else if (tablero[fila][columna].es_negra()) {
                    contadorNegro++;
                }
            }
        }

        if (contadorBlanco > contadorNegro) {
            cout << "Jugador Blanco gana con " << contadorBlanco << " fichas." << endl;
        } else if (contadorNegro > contadorBlanco) {
            cout << "Jugador Negro gana con " << contadorNegro << " fichas." << endl;
        } else {
            cout << "Empate. Ambos jugadores tienen " << contadorBlanco << " fichas." << endl;
        }
    }
};

void Jugador::hacer_jugada(Tablero& tablero, int fila, int columna) {
    if (fila < 0 || fila >= Tablero::tamaño || columna < 0 || columna >= Tablero::tamaño) {
            cout << "Jugada no valida. Fuera de limites del tablero." << endl;
            return;
        }

    if (tablero.hacer_jugada(fila, columna, *this)) {
        cout << "Jugada exitosa." << endl;
    } else {
        cout << "Jugada no válida. Intentalo de nuevo." << endl;
    }

    int direcciones[8][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

    bool jugada_valida = false;

    for (const auto& direccion : direcciones) {
        int fila_actual = fila + direccion[0];
        int columna_actual = columna + direccion[1];
        bool fichas_a_cambiar = false;

        while (fila_actual >= 0 && fila_actual < Tablero::tamaño && columna_actual >= 0 && columna_actual < Tablero::tamaño) {
            Casilla& casilla_actual = tablero.tablero[fila_actual][columna_actual];

            if (casilla_actual.esta_vacia()) {
                break;
            } else if (casilla_actual.es_del_oponente(obtener_color())) {
                fichas_a_cambiar = true;
            } else if (casilla_actual.es_del_jugador(obtener_color())) {
                if (fichas_a_cambiar) {
                    jugada_valida = true;

                    while (fila_actual != fila || columna_actual != columna) {
                        fila_actual -= direccion[0];
                        columna_actual -= direccion[1];
                        tablero.tablero[fila_actual][columna_actual].establecer_estado(Casilla::Estado::blanca);

                    }
                }
                break;
            }

            fila_actual += direccion[0];
            columna_actual += direccion[1];
        }
    }

    if (jugada_valida) {
        tablero.tablero[fila][columna].establecer_estado(Casilla::Estado::blanca);
    } else {
        cout << "Jugada no valida. No se han capturado fichas del oponente." << endl;
    }
}

// Implementación de las funciones miembro de Casilla
bool Casilla::es_del_oponente(Jugador::Color color) const {
    return (color == Jugador::Color::blanco && estado == Estado::negra) ||
           (color == Jugador::Color::negro && estado == Estado::blanca);
}

bool Casilla::es_del_jugador(Jugador::Color color) const {
    return (color == Jugador::Color::blanco && estado == Estado::blanca) ||
           (color == Jugador::Color::negro && estado == Estado::negra);
}

class Juego {
public:
    Tablero tablero;
    Jugador jugadorBlanco;
    Jugador jugadorNegro;

    Juego() : jugadorBlanco(Jugador(Jugador::Color::blanco)), jugadorNegro(Jugador(Jugador::Color::negro)) {
        tablero.inicializar_tablero();
    }

    void jugar_partida();

        void guardar_resultados();

        string obtener_fecha_actual();
};

void Juego::jugar_partida() {
    int turno = 0;
    while (true) {
        Jugador& jugador_actual = (turno % 2 == 0) ? jugadorBlanco : jugadorNegro;
        Jugador& jugador_oponente = (turno % 2 == 0) ? jugadorNegro : jugadorBlanco;

        tablero.mostrar_tablero();
        cout << "Turno del Jugador " << (jugador_actual.color == Jugador::Color::blanco ? "Blanco" : "Negro") << endl;

        if (tablero.jugador_puede_jugar(jugador_actual)) {
            int fila, columna;

            // Solicitar al jugador que ingrese su movimiento
            cout << "Ingresa tu jugada (fila columna): ";
            cin >> fila >> columna;

            if (tablero.hacer_jugada(fila, columna, jugador_actual)) {
                turno++;
            } else {
                cout << "Jugada no valida. Intentalo de nuevo." << endl;
            }
        } else {
            cout << "El jugador no puede hacer una jugada. Pasando al siguiente turno." << endl;
            turno++;
        }

        if (turno >= Tablero::tamaño * Tablero::tamaño || (!tablero.jugador_puede_jugar(jugadorBlanco) && !tablero.jugador_puede_jugar(jugadorNegro))) {
            break;
        }
    }

    tablero.determinar_ganador(jugadorBlanco, jugadorNegro);
    tablero.mostrar_tablero();
}


void Juego::guardar_resultados() {
    ofstream archivo_resultados("resultados.txt", ios::app);
    if (archivo_resultados.is_open()) {
        archivo_resultados << "Fecha: " << obtener_fecha_actual() << endl;
        archivo_resultados << "Jugador Blanco: " << (jugadorBlanco.color == Jugador::Color::blanco ? "Blanco" : "Negro") << endl;
        archivo_resultados << "Jugador Negro: " << (jugadorNegro.color == Jugador::Color::blanco ? "Blanco" : "Negro") << endl;
        archivo_resultados << "Fin de la partida" << endl;
        archivo_resultados.close();
    } else {
        cout << "No se pudo abrir el archivo de resultados." << endl;
    }
}

string Juego::obtener_fecha_actual() {
    time_t tiempo_actual = time(nullptr);
    struct tm* tiempo_info = localtime(&tiempo_actual);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tiempo_info);
    return buffer;
}

int main() {
    char continuar;

    do {
        Juego juego;
        juego.jugar_partida();
        juego.guardar_resultados();
        cout << "Desea jugar otra partida? (S/N): ";
        cin >> continuar;
    } while (continuar == 'S' || continuar == 's');

    return 0;
}

