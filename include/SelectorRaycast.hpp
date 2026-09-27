#pragma once
#include <algorithm>
#include "Vector3.hpp"
#include "Matriz4x4.hpp"
#include "CajaColision.hpp"

struct Rayo {
    Vector3 origen;
    Vector3 direccion;
};

class SelectorRaycast {
public:
    static Rayo ObtenerRayoDesdePantalla(int x, int y, int ancho, int alto, const Matriz4x4& vista, const Matriz4x4& proyeccion) {
        // 1. Convertir coordenadas de pantalla a NDC (Normalized Device Coordinates: [-1, 1])
        float xNdc = (2.0f * static_cast<float>(x)) / static_cast<float>(ancho) - 1.0f;
        float yNdc = 1.0f - (2.0f * static_cast<float>(y)) / static_cast<float>(alto);

        // 2. Obtener la posición del origen del rayo (Cámara) mediante la inversa de la matriz de vista
        Matriz4x4 invVista = vista.ObtenerInversa();
        Vector3 origenRayo = Vector3(
            invVista.Elementos[0][3],
            invVista.Elementos[1][3],
            invVista.Elementos[2][3]
        );

        // 3. Transformación inversa de proyección y vista para obtener la dirección en el espacio del mundo
        // Coordenadas en espacio de recorte (Clip Space) para el plano cercano (Z = -1)
        float fovX = proyeccion.Elementos[0][0];
        float fovY = proyeccion.Elementos[1][1];

        Vector3 dirVisualizacionLocal(xNdc / fovX, yNdc / fovY, -1.0f);
        
        // Rotar la dirección local al espacio mundial usando la matriz de vista inversa
        Vector3 dirRayo(
            invVista.Elementos[0][0] * dirVisualizacionLocal.X + invVista.Elementos[0][1] * dirVisualizacionLocal.Y + invVista.Elementos[0][2] * dirVisualizacionLocal.Z,
            invVista.Elementos[1][0] * dirVisualizacionLocal.X + invVista.Elementos[1][1] * dirVisualizacionLocal.Y + invVista.Elementos[1][2] * dirVisualizacionLocal.Z,
            invVista.Elementos[2][0] * dirVisualizacionLocal.X + invVista.Elementos[2][1] * dirVisualjoner.Y + invVista.Elementos[2][2] * dirVisualizacionLocal.Z
        );

        return { origenRayo, dirRayo.ObtenerNormalizado() };
    }

    static bool IntersectaAABB(const Rayo& rayo, const CajaColision& aabb, const Vector3& posObjeto, float& t) {
        Vector3 min = aabb.min + posObjeto;
        Vector3 max = aabb.max + posObjeto;

        float tmin = (min.X - rayo.origen.X) / (rayo.direccion.X == 0.0f ? 0.0001f : rayo.direccion.X);
        float tmax = (max.X - rayo.origen.X) / (rayo.direccion.X == 0.0f ? 0.0001f : rayo.direccion.X);

        if (tmin > tmax) std::swap(tmin, tmax);

        float tymin = (min.Y - rayo.origen.Y) / (rayo.direccion.Y == 0.0f ? 0.0001f : rayo.direccion.Y);
        float tymax = (max.Y - rayo.origen.Y) / (rayo.direccion.Y == 0.0f ? 0.0001f : rayo.direccion.Y);

        if (tymin > tymax) std::swap(tymin, tymax);

        if ((tmin > tymax) || (tymin > tmax)) return false;

        if (tymin > tmin) tmin = tymin;
        if (tymax < tmax) tmax = tymax;

        float tzmin = (min.Z - rayo.origen.Z) / (rayo.direccion.Z == 0.0f ? 0.0001f : rayo.direccion.Z);
        float tzmax = (max.Z - rayo.origen.Z) / (rayo.direccion.Z == 0.0f ? 0.0001f : rayo.direccion.Z);

        if (tzmin > tzmax) std::swap(tzmin, tzmax);

        if ((tmin > tzmax) || (tzmin > tmax)) return false;

        if (tzmin > tmin) tmin = tzmin;

        t = tmin;
        return t >= 0.0f;
    }
};
