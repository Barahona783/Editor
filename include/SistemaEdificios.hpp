#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <iostream>
#include "Entidad.hpp"
#include "Componentes.hpp"
#include "GestorEntidades.hpp"

// Estructura de metadatos espaciales para la entrada y salida de un edificio
struct ComponenteEdificioInteractivo {
    std::string NombreEdificio;
    Vector3 PuertaEntrada;    // Coordenada exacta de la puerta en el exterior de la calle
    Vector3 PuntoInterior;    // Coordenada de llegada/spawn dentro del edificio
    float RadioInteraccion;   // Distancia máxima para detectar la entrada (ej. 3.0 unidades)
    bool EstaAbierto;         // Control de acceso al edificio

    ComponenteEdificioInteractivo(const std::string& nombre = "Edificio Comercial", 
                                  const Vector3& entrada = Vector3(0.0f, 0.0f, 0.0f), 
                                  const Vector3& interior = Vector3(0.0f, 0.0f, 5.0f), 
                                  float radio = 3.0f, 
                                  bool abierto = true)
        : NombreEdificio(nombre), 
          PuertaEntrada(entrada), 
          PuntoInterior(interior), 
          RadioInteraccion(radio), 
          EstaAbierto(abierto) {}
};

class SistemaEdificios {
private:
    std::unordered_map<IdentificadorEntidad, ComponenteEdificioInteractivo> m_EdificiosMap;

public:
    // Registra o asigna un edificio interactivo a una entidad del motor
    void AsignarEdificio(const Entidad& entidad, const ComponenteEdificioInteractivo& edificio) {
        m_EdificiosMap[entidad.ObtenerID()] = edificio;
        std::cout << "[SistemaEdificios] Edificio registrado: " << edificio.NombreEdificio 
                  << " (ID Entidad: " << entidad.ObtenerID() << ")\n";
    }

    // Obtiene los datos del edificio si la entidad cuenta con él
    ComponenteEdificioInteractivo* ObtenerEdificio(const Entidad& entidad) {
        auto iter = m_EdificiosMap.find(entidad.ObtenerID());
        return (iter != m_EdificiosMap.end()) ? &(iter->second) : nullptr;
    }

    // Evalúa si un agente (NPC o Jugador) está lo suficientemente cerca de la puerta para interactuar o entrar
    void ActualizarYVerificarProximidadAgentes(GestorEntidades& gestor) {
        const auto& entidades = gestor.ObtenerTodasLasEntidades();

        for (const auto& entidadEdificio : m_EdificiosMap) {
            IdentificadorEntidad idEdificio = entidadEdificio.first;
            const auto& datosEdificio = entidadEdificio.second;

            if (!datosEdificio.EstaAbierto) continue;

            // Buscamos la transformación física del edificio si la tuviera asignada
            ComponenteTransformacion* transEdificio = gestor.ObtenerTransformacion(Entidad(idEdificio));
            Vector3 centroPuertaReal = datosEdificio.PuertaEntrada;
            
            if (transEdificio) {
                centroPuertaReal = transEdificio->Posicion + datosEdificio.PuertaEntrada;
            }

            // Comparamos la distancia de todos los agentes móviles (NPCs/Jugador) frente a la puerta
            for (const auto& entidadAgente : entidades) {
                if (entidadAgente.ObtenerID() == idEdificio) continue;

                ComponenteTransformacion* transAgente = gestor.ObtenerTransformacion(entidadAgente);
                if (transAgente) {
                    float dx = transAgente->Posicion.X - centroPuertaReal.X;
                    float dy = transAgente->Posicion.Y - centroPuertaReal.Y;
                    float dz = transAgente->Posicion.Z - centroPuertaReal.Z;
                    float distanciaSq = (dx * dx) + (dy * dy) + (dz * dz);
                    float radioSq = datosEdificio.RadioInteraccion * datosEdificio.RadioInteraccion;

                    // Si el agente entra en el radio de la puerta, se puede desencadenar la lógica de entrada
                    if (distanciaSq <= radioSq) {
                        // El agente ha localizado la puerta y está en rango de acceso
                    }
                }
            }
        }
    }
};
