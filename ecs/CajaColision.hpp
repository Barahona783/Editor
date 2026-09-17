#pragma once
#include "Vector3.hpp"

struct CajaColision {
    Vector3 Minimo;
    Vector3 Maximo;

    CajaColision(
        Vector3 minimo = Vector3(-0.5f, -0.5f, -0.5f),
        Vector3 maximo = Vector3(0.5f, 0.5f, 0.5f)
    ) : Minimo(minimo), Maximo(maximo) {}

    // Evalúa la intersección entre dos cajas delimitadoras AABB
    bool ColisionaCon(const CajaColision& otra, const Vector3& posEsta, const Vector3& posOtra) const {
        Vector3 estaMin = posEsta + Minimo;
        Vector3 estaMax = posEsta + Maximo;
        Vector3 otraMin = posOtra + otra.Minimo;
        Vector3 otraMax = posOtra + otra.Maximo;

        return (estaMin.X <= otraMax.X && estaMax.X >= otraMin.X) &&
               (estaMin.Y <= otraMax.Y && estaMax.Y >= otraMin.Y) &&
               (estaMin.Z <= otraMax.Z && estaMax.Z >= otraMin.Z);
    }
};
