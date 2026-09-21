#pragma once
#include "GestorEntidades.hpp"
#include "Componentes.hpp"
#include <vector>
#include <cmath>
#include <iostream>

// Componente para asignar una entidad a una coordenada de sector (Chunk) en el mundo abierto
struct ComponenteSector {
    int ChunkX;
    int ChunkZ;
    bool ActivoEnMemoria;

    ComponenteSector(int chunkX = 0, int chunkZ = 0) 
        : ChunkX(chunkX), ChunkZ(chunkZ), ActivoEnMemoria(true) {}
};

// Sistema encargado de gestionar el streaming dinámico del mundo abierto
class SistemaMundoAbierto {
private:
    float m_DistanciaCorte; // Radio máximo de visibilidad y procesamiento de chunks

public:
    SistemaMundoAbierto(float distanciaCorte = 50.0f) : m_DistanciaCorte(distanciaCorte) {}

    void EstablecerDistanciaCorte(float distancia) {
        m_DistanciaCorte = distancia;
    }

    // Actualiza el estado de las entidades en función de la posición actual del jugador/cámara
    void ActualizarStreaming(GestorEntidades& gestor, float jugadorX, float jugadorZ) {
        const auto& entidades = gestor.ObtenerTodasLasEntidades();

        for (const auto& entidad : entidades) {
            ComponenteTransformacion* transform = gestor.ObtenerTransformacion(entidad);
            ComponenteSector* sector = gestor.ObtenerComponente<ComponenteSector>(entidad);

            if (transform && sector) {
                // Calcular la distancia euclidiana plana (X-Z) entre el jugador y la entidad
                float deltaX = transform->Posicion.X - jugadorX;
                float deltaZ = transform->Posicion.Z - jugadorZ;
                float distancia = std::sqrt(deltaX * deltaX + deltaZ * deltaZ);

                // Si la entidad supera la distancia de corte, se marca como inactiva (ahorro de CPU/Memoria)
                if (distancia > m_DistanciaCorte) {
                    if (sector->ActivoEnMemoria) {
                        sector->ActivoEnMemoria = false;
                        // Aquí el motor puede pausar físicas o renderizado para este objeto
                    }
                } else {
                    if (!sector->ActivoEnMemoria) {
                        sector->ActivoEnMemoria = true;
                        // Se reactiva el objeto al volver al rango del jugador
                    }
                }
            }
        }
    }
};
