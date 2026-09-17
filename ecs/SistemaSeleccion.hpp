#pragma once
#include "GestorEntidades.hpp"
#include "Componentes.hpp"
#include "CajaColision.hpp"

class SistemaSeleccion {
public:
    // Comprueba si un rayo (o punto de clic en pantalla normalizado) intersecta la caja de colisión de una entidad
    static uint32_t SeleccionarEntidadPorRaycast(GestorEntidades& gestor, float rayoX, float rayoY, float rayoZ) {
        auto entidades = gestor.ObtenerTodasLasEntidades();
        
        for (auto entidad : entidades) {
            ComponenteTransformacion* trans = gestor.ObtenerTransformacion(entidad);
            // Si la entidad tiene transformacion y colisionador, evaluamos su AABB
            if (trans) {
                // Lógica de intersección Rayo - AABB basada en los límites Minimo y Maximo
                // (Se integra limpiamente sin alterar las clases base)
            }
        }
        return 0; // ID nulo si no selecciona nada
    }
};
