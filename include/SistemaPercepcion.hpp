#pragma once
#include <vector>
#include <cmath>
#include <iostream>
#include "Entidad.hpp"
#include "Componentes.hpp"
#include "GestorEntidades.hpp"

// Componente para dotar a las entidades (NPCs, Conductores, Jugador) de un radio de percepción sensorial
struct ComponentePercepcion {
    float RadioVision;          // Distancia máxima de alcance visual o proximidad (ej. 15.0 unidades)
    float AnguloVisionGrados;   // Campo de visión frontal (ej. 90.0 grados para visión directa, 360.0 para omnidireccional)
    bool AlertaActiva;          // Bandera que indica si ha detectado a otra entidad relevante
    IdentificadorEntidad EntidadObjetivoDetectada; // ID de la entidad que está en su campo visual

    ComponentePercepcion(float radio = 15.0f, float angulo = 360.0f)
        : RadioVision(radio), 
          AnguloVisionGrados(angulo), 
          AlertaActiva(false), 
          EntidadObjetivoDetectada(0) {}
};

class SistemaPercepcion {
private:
    // Almacena opcionalmente los componentes de percepción asociados por entidad
    std::unordered_map<IdentificadorEntidad, ComponentePercepcion> m_PercepcionesMap;

public:
    // Registra o asigna el componente de percepción a una entidad
    void AsignarPercepcion(const Entidad& entidad, const ComponentePercepcion& percepcion) {
        m_PercepcionesMap[entidad.ObtenerID()] = percepcion;
    }

    ComponentePercepcion* ObtenerPercepcion(const Entidad& entidad) {
        auto iter = m_PercepcionesMap.find(entidad.ObtenerID());
        return (iter != m_PercepcionesMap.end()) ? &(iter->second) : nullptr;
    }

    // Actualiza las comprobaciones de línea de visión y proximidad entre todas las entidades activas
    void Actualizar(GestorEntidades& gestor) {
        const auto& entidades = gestor.ObtenerTodasLasEntidades();

        // Evaluamos cada entidad que posea un componente de percepción (ej. vehículos detectando peatones)
        for (auto& parPercepcion : m_PercepcionesMap) {
            IdentificadorEntidad idObservador = parPercepcion.first;
            ComponentePercepcion& percepcionObs = parPercepcion.second;

            ComponenteTransformacion* transObs = gestor.ObtenerTransformacion(Entidad(idObservador));
            if (!transObs) continue;

            bool detectadoEnFrame = false;
            IdentificadorEntidad idMasCercano = 0;
            float menorDistanciaSq = percepcionObs.RadioVision * percepcionObs.RadioVision;

            // Comparamos contra el resto de las entidades del mundo
            for (const auto& entidadTarget : entidades) {
                if (entidadTarget.ObtenerID() == idObservador) continue;

                ComponenteTransformacion* transTarget = gestor.ObtenerTransformacion(entidadTarget);
                if (!transTarget) continue;

                float dx = transTarget->Posicion.X - transObs->Posicion.X;
                float dz = transTarget->Posicion.Z - transObs->Posicion.Z;
                float distanciaSq = (dx * dx) + (dz * dz);

                // Verificamos si entra dentro del radio de percepción configurado
                if (distanciaSq <= menorDistanciaSq) {
                    menorDistanciaSq = distanciaSq;
                    detectadoEnFrame = true;
                    idMasCercano = entidadTarget.ObtenerID();
                }
            }

            // Actualizamos el estado interno de la percepción del agente
            percepcionObs.AlertaActiva = detectadoEnFrame;
            percepcionObs.EntidadObjetivoDetectada = idMasCercano;
        }
    }
};
