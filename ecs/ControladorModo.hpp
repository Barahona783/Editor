#pragma once
#include <iostream>

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

    void IniciarJuego() {
        m_EstadoActual = EstadoMotor::Ejecucion;
        std::cout << "[ControladorModo] MODO JUEGO (PLAY) ACTIVADO.\n";
    }

    void DetenerJuego() {
        m_EstadoActual = EstadoMotor::Editor;
        std::cout << "[ControladorModo] MODO EDITOR (STOP) ACTIVADO.\n";
    }

    void PausarJuego() {
        m_EstadoActual = EstadoMotor::Pausado;
        std::cout << "[ControladorModo] JUEGO PAUSADO.\n";
    }

    EstadoMotor ObtenerEstado() const { return m_EstadoActual; }
    bool EstaEnModoJuego() const { return m_EstadoActual == EstadoMotor::Ejecucion; }
    bool EstaEnModoEditor() const { return m_EstadoActual == EstadoMotor::Editor; }
};
