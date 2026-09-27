#pragma once
#include "GestorEntidades.hpp"
#include "Componentes.hpp"
#include <vector>
#include <cmath>
#include <iostream>
#include <unordered_map>

// Componente de Sector Expansivo para la gestión de mundos abiertos masivos
struct ComponenteSector {
    int ChunkX;
    int ChunkZ;
    bool ActivoEnMemoria;
    bool AlertaProxima; // Zona de alta densidad para físicas, vehículos e IA avanzada

    ComponenteSector(int chunkX = 0, int chunkZ = 0) 
        : ChunkX(chunkX), ChunkZ(chunkZ), ActivoEnMemoria(true), AlertaProxima(false) {}
};

// Subsistema Core del Motor para el Streaming Masivo y Expansivo de Mundo Abierto
class SistemaMundoAbierto {
private:
    float m_RadioStreaming;      // Radio máximo de visibilidad y carga en unidades del mundo
    float m_RadioAlertaProxima;  // Radio interno para activación de físicas de alta prioridad
    float m_TamanoChunk;         // Dimensión métrica de cada sector/chunk expansivo (ej. 64.0 unidades)
    int m_RadioChunksVisibles;   // Radio de chunks activos alrededor del jugador

public:
    SistemaMundoAbierto(float radioStreaming = 256.0f, float tamanoChunk = 64.0f) 
        : m_RadioStreaming(radioStreaming), 
          m_RadioAlertaProxima(radioStreaming * 0.35f), 
          m_TamanoChunk(tamanoChunk) {
        m_RadioChunksVisibles = static_cast<int>(std::ceil(m_RadioStreaming / m_TamanoChunk));
    }

    void EstablecerRadioStreaming(float radio) {
        m_RadioStreaming = radio;
        m_RadioAlertaProxima = radio * 0.35f;
        m_RadioChunksVisibles = static_cast<int>(std::ceil(m_RadioStreaming / m_TamanoChunk));
    }

    float ObtenerRadioStreaming() const {
        return m_RadioStreaming;
    }

    float ObtenerTamanoChunk() const {
        return m_TamanoChunk;
    }

    // Actualiza dinámicamente el streaming expansivo y la persistencia de entidades en el mundo abierto
    void ActualizarStreaming(GestorEntidades& gestor, float jugadorX, float jugadorZ) {
        const auto& entidades = gestor.ObtenerTodasLasEntidades();

        // 1. Calcular en qué Chunk exacto se encuentra el jugador en este instante dentro del mundo expansivo
        int jugadorChunkX = static_cast<int>(std::floor((jugadorX + (m_TamanoChunk * 0.5f)) / m_TamanoChunk));
        int jugadorChunkZ = static_cast<int>(std::floor((jugadorZ + (m_TamanoChunk * 0.5f)) / m_TamanoChunk));

        for (const auto& entidad : entidades) {
            ComponenteTransformacion* transform = gestor.ObtenerTransformacion(entidad);
            ComponenteSector* sector = gestor.ObtenerComponente<ComponenteSector>(entidad);

            if (transform && sector) {
                // 2. Sincronizar automáticamente las coordenadas del chunk de la entidad según su posición global
                sector->ChunkX = static_cast<int>(std::floor((transform->Posicion.X + (m_TamanoChunk * 0.5f)) / m_TamanoChunk));
                sector->ChunkZ = static_cast<int>(std::floor((transform->Posicion.Z + (m_TamanoChunk * 0.5f)) / m_TamanoChunk));

                // 3. Calcular la distancia en chunks respecto al jugador para soportar expansión masiva
                int deltaChunkX = std::abs(sector->ChunkX - jugadorChunkX);
                int deltaChunkZ = std::abs(sector->ChunkZ - jugadorChunkZ);

                // Distancia euclidiana exacta en el plano X-Z del motor
                float deltaX = transform->Posicion.X - jugadorX;
                float deltaZ = transform->Posicion.Z - jugadorZ;
                float distanciaEuclidiana = std::sqrt(deltaX * deltaX + deltaZ * deltaZ);

                // 4. Lógica de Activación / Desactivación Expansiva (Streaming inteligente)
                bool dentroDelRadioDeCarga = (deltaChunkX <= m_RadioChunksVisibles && deltaChunkZ <= m_RadioChunksVisibles) && 
                                             (distanciaEuclidiana <= m_RadioStreaming);

                if (!dentroDelRadioDeCarga) {
                    if (sector->ActivoEnMemoria) {
                        sector->ActivoEnMemoria = false;
                        sector->AlertaProxima = false;
                        // El motor suspende la entidad para optimizar recursos en mundos gigantescos
                    }
                } else {
                    if (!sector->ActivoEnMemoria) {
                        sector->ActivoEnMemoria = true;
                        // El motor deserializa o reactiva la entidad al reingresar al radio activo
                    }

                    // Evaluación de la zona de alta prioridad (proximidad interactiva / tráfico / NPCs)
                    if (distanciaEuclidiana <= m_RadioAlertaProxima) {
                        sector->AlertaProxima = true;
                    } else {
                        sector->AlertaProxima = false;
                    }
                }
            }
        }
    }
};
