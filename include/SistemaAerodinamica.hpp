#pragma once
#include <vector>
#include <cmath>
#include <algorithm>
#include "Entidad.hpp"
#include "Componentes.hpp"
#include "GestorEntidades.hpp"

class SistemaAerodinamica {
private:
    Vector3 m_VientoGlobal; // Dirección y fuerza del viento ambiental en el mundo abierto
    float m_DensidadFluido;  // Densidad del aire o fluido (ej. 1.225 kg/m³ a nivel del mar)

public:
    SistemaAerodinamica(const Vector3& vientoInicial = Vector3(2.0f, 0.0f, 0.5f), float densidad = 1.225f)
        : m_VientoGlobal(vientoInicial), m_DensidadFluido(densidad) {}

    // Permite modificar dinámicamente las condiciones meteorológicas del viento en el mundo abierto
    void EstablecerVientoGlobal(const Vector3& nuevoViento) {
        m_VientoGlobal = nuevoViento;
    }

    // Calcula y aplica las fuerzas aerodinámicas (Arrastre과 Sustentación) de forma universal
    void Actualizar(GestorEntidades& gestor, float deltaTime) {
        const auto& entidades = gestor.ObtenerTodasLasEntidades();

        for (const auto& entidad : entidades) {
            ComponenteAerodinamico* aero = gestor.ObtenerComponente<ComponenteAerodinamico>(entidad);
            ComponenteTransformacion* trans = gestor.ObtenerTransformacion(entidad);
            ComponenteCuerpoRigido* rigido = gestor.ObtenerCuerpoRigido(entidad);

            if (!aero || !aero->Activo || !trans || !rigido || !rigido->EsDinamico) {
                continue;
            }

            // Velocidad relativa considerando el movimiento de la entidad frente al viento global del mundo
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
            if (rigido->Masa > 0.0f) {
                float aceleracionSustentacion = fuerzaSustentacionMagnitud / rigido->Masa;

                // Modificamos sutilmente la posición según el impacto aerodinámico universal
                trans->Posicion.Y += aceleracionSustentacion * deltaTime * 0.1f;
            }
        }
    }
};
