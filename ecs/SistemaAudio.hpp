#pragma once
#include "GestorEntidades.hpp"
#include "GestorAudio.hpp"

class SistemaAudio {
public:
    void Actualizar(GestorEntidades& gestor) {
        auto& entidades = gestor.ObtenerTodasLasEntidades();

        for (const auto& entidad : entidades) {
            auto fuenteAudio = gestor.ObtenerFuenteAudio(entidad);

            if (fuenteAudio && fuenteAudio->DebeReproducir) {
                GestorAudio::ReproducirSonidoWAV(fuenteAudio->RutaArchivo, fuenteAudio->EnBucle);
                fuenteAudio->DebeReproducir = false; // Restablecer bandera tras activar
            }
        }
    }
};
