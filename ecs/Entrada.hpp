#pragma once
#include <unordered_map>

enum class CodigoTecla {
    W, A, S, D, Espacio, Escape, Izquierda, Derecha, Arriba, Abajo
};

class Entrada {
private:
    static std::unordered_map<CodigoTecla, bool> s_Teclas;

public:
    static void EstablecerEstadoTecla(CodigoTecla tecla, bool presionada) {
        s_Teclas[tecla] = presionada;
    }

    static bool EstaTeclaPresionada(CodigoTecla tecla) {
        auto iterador = s_Teclas.find(tecla);
        if (iterador != s_Teclas.end()) {
            return iterador->second;
        }
        return false;
    }
};

inline std::unordered_map<CodigoTecla, bool> Entrada::s_Teclas;
