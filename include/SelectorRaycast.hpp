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
        float xNdc = (2.0f * x) / (float)ancho - 1.0f;
        float yNdc = 1.0f - (2.0f * y) / (float)alto;

        Vector3 dirRayo = Vector3(xNdc, yNdc, -1.0f).Normalizar();
        Vector3 origenRayo = Vector3(-vista.m[12], -vista.m[13], -vista.m[14]);

        return { origenRayo, dirRayo };
    }

    static bool IntersectaAABB(const Rayo& rayo, const CajaColision& aabb, const Vector3& posObjeto, float& t) {
        Vector3 min = aabb.min + posObjeto;
        Vector3 max = aabb.max + posObjeto;

        float tmin = (min.x - rayo.origen.x) / (rayo.direccion.x == 0.0f ? 0.0001f : rayo.direccion.x);
        float tmax = (max.x - rayo.origen.x) / (rayo.direccion.x == 0.0f ? 0.0001f : rayo.direccion.x);

        if (tmin > tmax) std::swap(tmin, tmax);

        float tymin = (min.y - rayo.origen.y) / (rayo.direccion.y == 0.0f ? 0.0001f : rayo.direccion.y);
        float tymax = (max.y - rayo.origen.y) / (rayo.direccion.y == 0.0f ? 0.0001f : rayo.direccion.y);

        if (tymin > tymax) std::swap(tymin, tymax);

        if ((tmin > tymax) || (tymin > tmax)) return false;

        if (tymin > tmin) tmin = tymin;
        if (tymax < tmax) tmax = tymax;

        float tzmin = (min.z - rayo.origen.z) / (rayo.direccion.z == 0.0f ? 0.0001f : rayo.direccion.z);
        float tzmax = (max.z - rayo.origen.z) / (rayo.direccion.z == 0.0f ? 0.0001f : rayo.direccion.z);

        if (tzmin > tzmax) std::swap(tzmin, tzmax);

        if ((tmin > tzmax) || (tzmin > tmax)) return false;

        if (tzmin > tmin) tmin = tzmin;

        t = tmin;
        return t >= 0.0f;
    }
};
