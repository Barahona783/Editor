#pragma once
#include <vector>
#include <GL/gl.h>
#include "Vector3.hpp"
#include "GestorEntidades.hpp"

// ==========================================
// ESTRUCTURA DE PARTÍCULA INDIVIDUAL
// ==========================================
struct Particula {
    Vector3 posicion;
    Vector3 velocidad;
    float vidaRestante;
    float vidaTotal;
    float tamano;
    float alfa;
};

// ==========================================
// SISTEMA DE EMISIÓN Y SIMULACIÓN DE PARTÍCULAS
// ==========================================
class SistemaParticulas {
private:
    std::vector<Particula> m_Particulas;
    Vector3 m_OrigenEmisor;
    int m_MaxParticulas;

public:
    SistemaParticulas(int maxParticulas = 200) 
        : m_OrigenEmisor(0.0f, 0.0f, -5.0f), m_MaxParticulas(maxParticulas) {
        m_Particulas.reserve(m_MaxParticulas);
    }

    // Emite una nueva partícula si no se ha alcanzado el límite máximo permitido
    void EmitirParticula(const Vector3& origen, const Vector3& velocidad, float vida, float tamano) {
        if (m_Particulas.size() >= static_cast<size_t>(m_MaxParticulas)) return;

        Particula p;
        p.posicion = origen;
        p.velocidad = velocidad;
        p.vidaRestante = vida;
        p.vidaTotal = vida;
        p.tamano = tamano;
        p.alfa = 1.0f;

        m_Particulas.push_back(p);
    }

    // Actualiza la física, la vida útil y el desvanecimiento alfa de las partículas activas
    void Actualizar(float deltaTime) {
        for (auto it = m_Particulas.begin(); it != m_Particulas.end(); ) {
            it->vidaRestante -= deltaTime;

            if (it->vidaRestante <= 0.0f) {
                it = m_Particulas.erase(it);
            } else {
                // Actualizar física básica (Integración de Euler lineal)
                it->posicion.X += it->velocidad.X * deltaTime;
                it->posicion.Y += it->velocidad.Y * deltaTime;
                it->posicion.Z += it->velocidad.Z * deltaTime;

                // Transparencia gradual según la vida restante
                it->alfa = it->vidaRestante / it->vidaTotal;
                ++it;
            }
        }
    }

    // Renderiza las partículas utilizando blending aditivo para efectos luminosos
    void Renderizar() {
        if (m_Particulas.empty()) return;

        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE); // Renderizado aditivo para efectos de fuego/energía

        glPointSize(5.0f);
        glBegin(GL_POINTS);

        for (const auto& p : m_Particulas) {
            glColor4f(1.0f, 0.6f, 0.1f, p.alfa); // Color cálido radiante con atenuación alfa
            glVertex3f(p.posicion.X, p.posicion.Y, p.posicion.Z);
        }

        glEnd();

        glDisable(GL_BLEND);
    }

    void EstablecerOrigenEmisor(const Vector3& origen) {
        m_OrigenEmisor = origen;
    }

    Vector3 ObtenerOrigenEmisor() const {
        return m_OrigenEmisor;
    }
};
