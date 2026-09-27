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

    // Calcula y aplica las fuerzas aerodinámicas (Arrastre y Sustentación) de forma universal
    void Actualizar(GestorEntidades& gestor, float deltaTime) {
        const auto& entidades = gestor.ObtenerTodasLasEntidades();

        for (const auto& entidad : entidades) {
            ComponenteAerodinamico* aero = gestor.ObtenerComponente<ComponenteAerodinamico>(entidad);
            ComponenteTransformacion* trans = gestor.ObtenerTransformacion(entidad);
            ComponenteCuerpoRigido* rigido = gestor.ObtenerCuerpoRigido(entidad);

            if (!aero || !aero->Activo || !trans || !rigido || !rigido->EsDinamico) {
                continue;
            }

            // Velocidad relativa real: Diferencia entre el viento global y la velocidad del cuerpo rígido de la entidad
            Vector3 velocidadVientoRelativo = Vector3(
                m_VientoGlobal.X - rigido->Velocidad.X,
                m_VientoGlobal.Y - rigido->Velocidad.Y,
                m_VientoGlobal.Z - rigido->Velocidad.Z
            );

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
                // Aceleración vertical derivada de la sustentación o downforce
                float aceleracionSustentacion = fuerzaSustentacionMagnitud / rigido->Masa;

                // Modificamos sutilmente la posición según el impacto aerodinámico universal
                trans->Posicion.Y += aceleracionSustentacion * deltaTime * 0.1f;
                
                // Aplicamos también un pequeño arrastre directo sobre la velocidad del cuerpo rígido
                if (velocidadRelativaMagnitud > 0.001f) {
                    Vector3 direccionRelativa = Vector3(
                        velocidadVientoRelativo.X / velocidadRelativaMagnitud,
                        velocidadVientoRelativo.Y / velocidadRelativaMagnitud,
                        velocidadVientoRelativo.Z / velocidadRelativaMagnitud
                    );
                    float aceleracionArrastre = fuerzaArrastreMagnitud / rigido->Masa;
                    rigido->Velocidad.X += direccionRelativa.X * aceleracionArrastre * deltaTime;
                    rigido->Velocidad.Z += direccionRelativa.Z * aceleracionArrastre * deltaTime;
                }
            }
        }
    }
};
