#pragma once
#include <GL/gl.h>
#include <string>

class SistemaRendimiento {
private:
    bool m_MostrarPanelRendimiento;
    int m_LimiteFPSSeleccionado; // 0 = Sin límite, 30, 60, 120
    float m_TemperaturaSimulada;  // Métrica universal de estado térmico
    float m_UsoMemoriaMB;

public:
    SistemaRendimiento() 
        : m_MostrarPanelRendimiento(true), 
          m_LimiteFPSSeleccionado(60), 
          m_TemperaturaSimulada(35.0f), 
          m_UsoMemoriaMB(45.2f) {}

    void AlternarPanel() {
        m_MostrarPanelRendimiento = !m_MostrarPanelRendimiento;
    }

    bool EstaActivo() const {
        return m_MostrarPanelRendimiento;
    }

    // Método universal que se dibujará idéntico en Android y PC
    void RenderizarPanelRendimiento() {
        if (!m_MostrarPanelRendimiento) return;

        // Fondo del panel de rendimiento (ej. esquina superior derecha o flotante)
        glColor4f(0.05f, 0.05f, 0.07f, 0.90f);
        glBegin(GL_QUADS);
            glVertex2f(950.0f, 60.0f);
            glVertex2f(1260.0f, 60.0f);
            glVertex2f(1260.0f, 250.0f);
            glVertex2f(950.0f, 250.0f);
        glEnd();

        // Aquí se renderizarán los textos o indicadores visuales universales:
        // 1. Estado Térmico / Temperatura
        // 2. Límite de FPS actual
        // 3. Uso de memoria del motor
    }

    void EstablecerLimiteFPS(int limite) {
        m_LimiteFPSSeleccionado = limite;
    }
};
