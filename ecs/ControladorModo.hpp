#pragma once
#include <iostream>

// ==========================================
// CONTROLADOR DE ESTADO DEL MOTOR (Editor / Play / Pause)
// ==========================================
enum class EstadoMotor {
    Editor,
    Ejecucion,
    Pausado
};

class ControladorModo {
private:
    EstadoMotor m_EstadoActual;

public:
    ControladorModo() : m_EstadoActual(EstadoMotor::Editor) {}

    // Cambia el motor al modo de juego o simulación activa
    void IniciarJuego() {
        m_EstadoActual = EstadoMotor::Ejecucion;
        #ifdef _DEBUG
        std::cout << "[ControladorModo] MODO JUEGO (PLAY) ACTIVADO.\n";
        #endif
    }

    // Detiene la simulación y regresa al modo de edición del mundo
    void DetenerJuego() {
        m_EstadoActual = EstadoMotor::Editor;
        #ifdef _DEBUG
        std::cout << "[ControladorModo] MODO EDITOR (STOP) ACTIVADO.\n";
        #endif
    }

    // Pausa temporalmente la ejecución lógica del motor
    void PausarJuego() {
        m_EstadoActual = EstadoMotor::Pausado;
        #ifdef _DEBUG
        std::cout << "[ControladorModo] JUEGO PAUSADO.\n";
        #endif
    }

    // Consultas de estado de alto rendimiento
    EstadoMotor ObtenerEstado() const { return m_EstadoActual; }
    bool EstaEnModoJuego() const { return m_EstadoActual == EstadoMotor::Ejecucion; }
    bool EstaEnModoEditor() const { return m_EstadoActual == EstadoMotor::Editor; }
    bool EstaPausado() const { return m_EstadoActual == EstadoMotor::Pausado; }
};
