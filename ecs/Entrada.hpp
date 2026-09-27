#pragma once
#include <unordered_map>

// ==========================================
// SISTEMA DE ENTRADA Y GESTIÓN DE INPUT
// ==========================================
enum class CodigoTecla {
    W, A, S, D, Espacio, Escape, Izquierda, Derecha, Arriba, Abajo
};

class Entrada {
private:
    static std::unordered_map<CodigoTecla, bool> s_Teclas;

public:
    // Actualiza de manera síncrona el estado de presión de una tecla específica
    static void EstablecerEstadoTecla(CodigoTecla tecla, bool presionada) {
        s_Teclas[tecla] = presionada;
    }

    // Consulta de ultra bajo costo para verificar si una tecla se encuentra presionada
    static bool EstaTeclaPresionada(CodigoTecla tecla) {
        auto iterador = s_Teclas.find(tecla);
        if (iterador != s_Teclas.end()) {
            return iterador->second;
        }
        return false;
    }
};

// Inicialización inline del contenedor estático para evitar dependencias de archivos .cpp
inline std::unordered_map<CodigoTecla, bool> Entrada::s_Teclas;
