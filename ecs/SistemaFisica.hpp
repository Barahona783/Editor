#pragma once
#include "GestorEntidades.hpp"
#include "Vector3.hpp"

class SistemaFisica {
private:
    Vector3 m_Gravedad;

public:
    SistemaFisica(Vector3 gravedad = Vector3(0.0f, -9.81f, 0.0f))
        : m_Gravedad(gravedad) {}

    void Actualizar(GestorEntidades& gestor, float tiempoDelta) {
        auto& entidades = gestor.ObtenerTodasLasEntidades();

        for (const auto& entidad : entidades) {
            auto transformacion = gestor.ObtenerTransformacion(entidad);
            auto cuerpoRigido = gestor.ObtenerCuerpoRigido(entidad);

            if (transformacion && cuerpoRigido && cuerpoRigido->EsDinamico) {
                // Aplicar gravedad y aceleración (Integración de Euler)
                cuerpoRigido->Velocidad = cuerpoRigido->Velocidad + (m_Gravedad * cuerpoRigido->EscalaGravedad * tiempoDelta);
                transformacion->Posicion = transformacion->Posicion + (cuerpoRigido->Velocidad * tiempoDelta);

                // Detección simple con el plano del suelo (Y = 0)
                if (transformacion->Posicion.Y <= 0.0f) {
                    transformacion->Posicion.Y = 0.0f;
                    cuerpoRigido->Velocidad.Y = 0.0f;
                    cuerpoRigido->EnElSuelo = true;
                } else {
                    cuerpoRigido->EnElSuelo = false;
                }
            }
        }
    }

    void EstablecerGravedad(const Vector3& gravedad) {
        m_Gravedad = gravedad;
    }
};
