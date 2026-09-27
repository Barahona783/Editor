#pragma once
#include <GL/gl.h>
#include <string>
#include <algorithm>

// ==========================================
// SISTEMA DE MONITOREO Y RENDIMIENTO DEL MOTOR
// ==========================================
class SistemaRendimiento {
private:
    bool m_MostrarPanelRendimiento;
    int m_LimiteFPSSeleccionado; // 0 = Sin límite, 30, 60, 120
    float m_TemperaturaSimulada;  // Métrica universal de estado térmico (Celsius)
    float m_UsoMemoriaMB;         // Uso estimado de memoria RAM/VRAM en MB

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

    // Simula la fluctuación y actualización de métricas en función del tiempo de ejecución
    void ActualizarMetricas(float deltaTime, float cargaActualSistema) {
        (void)deltaTime;
        // Simula variación térmica basada en la carga del sistema
        float temperaturaObjetivo = 35.0f + (cargaActualSistema * 25.0f);
        m_TemperaturaSimulada += (temperaturaObjetivo - m_TemperaturaSimulada) * 0.05f;
    }

    // Método universal de renderizado del panel superpuesto (HUD) para PC y Android
    void RenderizarPanelRendimiento() {
        if (!m_MostrarPanelRendimiento) return;

        // Desactivar iluminación temporalmente para dibujar elementos HUD 2D planos
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Fondo semitransparente del panel de rendimiento (esquina superior derecha)
        glColor4f(0.05f, 0.05f, 0.07f, 0.85f);
        glBegin(GL_QUADS);
            glVertex2f(950.0f, 60.0f);
            glVertex2f(1260.0f, 60.0f);
            glVertex2f(1260.0f, 250.0f);
            glVertex2f(950.0f, 250.0f);
        glEnd();

        glDisable(GL_BLEND);

        // Espacio reservado para el pipeline de texto del motor:
        // 1. Estado Térmico / Temperatura (m_TemperaturaSimulada)
        // 2. Límite de FPS actual (m_LimiteFPSSeleccionado)
        // 3. Uso de memoria del motor (m_UsoMemoriaMB)
    }

    void EstablecerLimiteFPS(int limite) {
        m_LimiteFPSSeleccionado = std::max(0, limite);
    }

    int ObtenerLimiteFPS() const {
        return m_LimiteFPSSeleccionado;
    }

    float ObtenerTemperatura() const {
        return m_TemperaturaSimulada;
    }

    float ObtenerUsoMemoria() const {
        return m_UsoMemoriaMB;
    }
};
