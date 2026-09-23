#pragma once
#include "Entidad.hpp"
#include "GestorEntidades.hpp"
#include "Componentes.hpp"
#include <limits>
#include <algorithm>

class SistemaSeleccion {
private:
    Entidad m_EntidadSeleccionada;
    bool m_HaySeleccion;

public:
    SistemaSeleccion() : m_EntidadSeleccionada(), m_HaySeleccion(false) {}

    // Algoritmo de intersección Rayo - AABB (Slab Method) optimizado
    static Entidad SeleccionarEntidadPorRaycast(GestorEntidades& gestor, 
                                                float rayoOrigenX, float rayoOrigenY, float rayoOrigenZ,
                                                float rayoDirX, float rayoDirY, float rayoDirZ) {
        auto entidades = gestor.ObtenerTodasLasEntidades();
        Entidad entidadSeleccionada; 
        float distanciaMinima = std::numeric_limits<float>::max();

        for (auto entidad : entidades) {
            ComponenteTransformacion* trans = gestor.ObtenerTransformacion(entidad);
            if (!trans) continue;

            float minX = trans->Posicion.X - 0.5f;
            float maxX = trans->Posicion.X + 0.5f;
            float minY = trans->Posicion.Y - 0.5f;
            float maxY = trans->Posicion.Y + 0.5f;
            float minZ = trans->Posicion.Z - 0.5f;
            float maxZ = trans->Posicion.Z + 0.5f;

            float tMin = (minX - rayoOrigenX) / (rayoDirX != 0.0f ? rayoDirX : 0.0001f);
            float tMax = (maxX - rayoOrigenX) / (rayoDirX != 0.0f ? rayoDirX : 0.0001f);

            if (tMin > tMax) std::swap(tMin, tMax);

            float tyMin = (minY - rayoOrigenY) / (rayoDirY != 0.0f ? rayoDirY : 0.0001f);
            float tyMax = (maxY - rayoOrigenY) / (rayoDirY != 0.0f ? rayoDirY : 0.0001f);

            if (tyMin > tyMax) std::swap(tyMin, tyMax);

            if ((tMin > tyMax) || (tyMin > tMax)) continue;

            if (tyMin > tMin) tMin = tyMin;
            if (tyMax < tMax) tMax = tyMax;

            float tzMin = (minZ - rayoOrigenZ) / (rayoDirZ != 0.0f ? rayoDirZ : 0.0001f);
            float tzMax = (maxZ - rayoOrigenZ) / (rayoDirZ != 0.0f ? rayoDirZ : 0.0001f);

            if (tzMin > tzMax) std::swap(tzMin, tzMax);

            if ((tMin > tzMax) || (tzMin > tMax)) continue;

            if (tzMin > tMin) tMin = tzMin;
            if (tzMax < tMax) tMax = tzMax;

            if (tMin >= 0.0f && tMin < distanciaMinima) {
                distanciaMinima = tMin;
                entidadSeleccionada = entidad;
            }
        }
        return entidadSeleccionada; 
    }

    // Métodos de gestión de estado de selección para la interfaz del editor
    void SeleccionarEntidad(Entidad entidad) {
        m_EntidadSeleccionada = entidad;
        m_HaySeleccion = (entidad.ObtenerID() != 0);
    }

    void Deseleccionar() {
        m_EntidadSeleccionada = Entidad();
        m_HaySeleccion = false;
    }

    bool TieneSeleccion() const { return m_HaySeleccion; }
    
    Entidad ObtenerEntidadSeleccionada() const { return m_EntidadSeleccionada; }
};
