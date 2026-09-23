#pragma once
#include <vector>
#include <cmath>
#include <algorithm>
#include "Entidad.hpp"
#include "Componentes.hpp"
#include "GestorEntidades.hpp"

// Componente universal de aerodinámica aplicable a vehículos, barcos, aviones o proyectiles
struct ComponenteAerodinamico {
    float CoeficienteArrastre; // Resistencia frontal al viento (ej. 0.30 para autos aerodinámicos, 0.8 para cajas)
    float AreaFrontal;        // Superficie de impacto del viento (metros cuadrados)
    float CoeficienteSustentacion; // Fuerza vertical o 'downforce' / 'lift' (positivo para alas, negativo para pegarse al suelo)
    bool Activo;

    ComponenteAerodinamico(float drag = 0.35f, float area = 2.0f, float lift = -0.5f)
        : CoeficienteArrastre(drag), 
          AreaFrontal(area), 
          CoeficienteSustentacion(lift), 
          Activo(true) {}
};

class SistemaAerodinamica {
private:
    Vector3 m_VientoGlobal; // Dirección y fuerza del viento ambiental en el mundo abierto (ej. soplando hacia el este)
    float m_DensidadFluido;  // Densidad del aire o fluido (ej. 1.225 kg/m³ a nivel del mar)

public:
    SistemaAerodinamica(const Vector3& vientoInicial = Vector3(2.0f, 0.0f, 0.5f), float densidad = 1.225f)
        : m_VientoGlobal(vientoInicial), m_DensidadFluido(densidad) {}

    // Permite modificar dinámicamente las condiciones meteorológicas del viento en el mundo abierto
    void EstablecerVientoGlobal(const Vector3& nuevoViento) {
        m_VientoGlobal = nuevoViento;
    }

    // Calcula y aplica las fuerzas aerodinámicas (Arrastre y Sustentación) de forma universal
    void Actualizar(GestorEntidades& gestor, float deltaTime) {
        const auto& entidades = gestor.ObtenerTodasLasEntidades();

        for (const auto& entidad : entidades) {
            ComponenteAerodinamico* aero = gestor.ObtenerComponente<ComponenteAerodinamico>(entidad);
            ComponenteTransformacion* trans = gestor.ObtenerTransformacion(entidad);
            ComponenteCuerpoRigido* rigido = gestor.ObtenerComponente<ComponenteCuerpoRigido>(entidad);

            if (!aero || !aero->Activo || !trans || !rigido || !rigido->EsDinamico) {
                continue;
            }

            // Velocidad relativa considerando el movimiento de la entidad frente al viento global del mundo
            // (En un motor completo, aquí se usaría la velocidad lineal real del cuerpo rígido)
            Vector3 velocidadVientoRelativo = m_VientoGlobal; 
            float velocidadRelativaMagnitud = std::sqrt(
                velocidadVientoRelativo.X * velocidadVientoRelativo.X +
                velocidadVientoRelativo.Y * velocidadVientoRelativo.Y +
                velocidadVientoRelativo.Z * velocidadVientoRelativo.Z
            );

            // Fórmula general de Arrastre (Drag): F_d = 0.5 * rho * v^2 * Cd * Area
            float fuerzaArrastreMagnitud = 0.5f * m_DensidadFluido * (velocidadRelativaMagnitud * velocidadRelativaMagnitud) 
                                           * aero->CoeficienteArrastre * aero->AreaFrontal;

            // Fórmula general de Sustentación/Downforce (Lift): F_l = 0.5 * rho * v^2 * Cl * Area
            float fuerzaSustentacionMagnitud = 0.5f * m_DensidadFluido * (velocidadRelativaMagnitud * velocidadRelativaMagnitud) 
                                             * aero->CoeficienteSustentacion * aero->AreaFrontal;

            // Aplicación de las fuerzas sobre los componentes físicos de la entidad
            // (El arrastre frena en contra del viento, la sustentación actúa verticalmente en el eje Y)
            if (rigido->Masa > 0.0f) {
                float aceleracionArrastre = fuerzaArrastreMagnitud / rigido->Masa;
                float aceleracionSustentacion = fuerzaSustentacionMagnitud / rigido->Masa;

                // Modificamos sutilmente la posición o velocidad simulada según el impacto aerodinámico universal
                trans->Posicion.Y += aceleracionSustentacion * deltaTime * 0.1f;
            }
        }
    }
};
