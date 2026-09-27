#pragma once
#include "GestorEntidades.hpp"
#include "GestorAudio.hpp"

// ==========================================
// SISTEMA DE GESTIÓN Y REPRODUCCIÓN DE AUDIO POR EVENTOS
// ==========================================
class SistemaAudio {
public:
    // Actualiza el estado de audio para todas las entidades activas en cada frame del motor
    void Actualizar(GestorEntidades& gestor) {
        const auto& entidades = gestor.ObtenerTodasLasEntidades();

        for (const auto& entidad : entidades) {
            auto fuenteAudio = gestor.ObtenerFuenteAudio(entidad);

            if (fuenteAudio && fuenteAudio->DebeReproducir) {
                GestorAudio::ReproducirSonidoWAV(fuenteAudio->RutaArchivo, fuenteAudio->EnBucle);
                fuenteAudio->DebeReproducir = false; // Restablecer bandera tras disparar el evento de audio
            }
        }
    }
};
