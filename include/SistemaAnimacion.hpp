#pragma once
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include "Entidad.hpp"
#include "Componentes.hpp"
#include "GestorEntidades.hpp"

class SistemaAnimacion {
public:
    // Actualiza la máquina de estados, el tiempo de reproducción y la interpolación (Blend Space) de cada entidad
    void Actualizar(GestorEntidades& gestor, float deltaTime) {
        const auto& entidades = gestor.ObtenerTodasLasEntidades();

        for (const auto& entidad : entidades) {
            ComponenteAnimacion* anim = gestor.ObtenerComponente<ComponenteAnimacion>(entidad);
            ComponenteTransformacion* trans = gestor.ObtenerTransformacion(entidad);

            if (!anim || !anim->EnReproduccion) {
                continue;
            }

            // Incrementar el tiempo actual del clip de animación
            anim->TiempoActual += deltaTime;

            // Manejo de bucles (looping) del clip activo
            if (anim->TiempoActual >= anim->Duracion) {
                if (anim->EnBucle) {
                    anim->TiempoActual = fmod(anim->TiempoActual, anim->Duracion);
                } else {
                    anim->TiempoActual = anim->Duracion;
                    anim->EnReproduccion = false;
                }
            }

            // Lógica de transición suave (Blend Space / State Machine)
            if (!anim->SiguienteEstado.empty() && anim->SiguienteEstado != anim->EstadoActual) {
                // Interpolar el peso de la mezcla hacia el estado destino
                anim->PesoBlend += anim->VelocidadMezcla * deltaTime;
                if (anim->PesoBlend >= 1.0f) {
                    // Completar la transición de estado
                    anim->EstadoActual = anim->SiguienteEstado;
                    anim->SiguienteEstado = "";
                    anim->PesoBlend = 0.0f;
                }
            } else {
                // Reducir gradualmente el peso de mezcla si no hay transición activa
                if (anim->PesoBlend > 0.0f) {
                    anim->PesoBlend -= anim->VelocidadMezcla * deltaTime;
                    if (anim->PesoBlend < 0.0f) anim->PesoBlend = 0.0f;
                }
            }
        }
    }

    // Permite disparar un cambio de estado con interpolación suave desde la lógica de juego o scripts
    void TransicionarA(ComponenteAnimacion& anim, const std::string& nuevoEstado, float velocidadMezcla = 5.0f) {
        if (anim.EstadoActual != nuevoEstado) {
            anim.SiguienteEstado = nuevoEstado;
            anim.VelocidadMezcla = velocidadMezcla;
            anim.PesoBlend = 0.0f; // Reinicia el blend para una interpolación fluida
        }
    }
};
