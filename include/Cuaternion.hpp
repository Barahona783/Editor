#pragma once
#include "Vector3.hpp"
#include <cmath>

struct Cuaternion {
    float W;
    float X;
    float Y;
    float Z;

    Cuaternion() : W(1.0f), X(0.0f), Y(0.0f), Z(0.0f) {}
    Cuaternion(float w, float x, float y, float z) : W(w), X(x), Y(y), Z(z) {}

    // Crear Cuaternión a partir de un Eje y un Ángulo (en radianes)
    static Cuaternion CrearDesdeEjeAngulo(const Vector3& eje, float anguloRadianes) {
        Vector3 ejeNormalizado = eje.ObtenerNormalizado();
        float mitadAngulo = anguloRadianes * 0.5f;
        float senoMitad = std::sin(mitadAngulo);

        return Cuaternion(
            std::cos(mitadAngulo),
            ejeNormalizado.X * senoMitad,
            ejeNormalizado.Y * senoMitad,
            ejeNormalizado.Z * senoMitad
        );
    }

    // Multiplicación de Cuaterniones (Combina rotaciones)
    Cuaternion operator*(const Cuaternion& q) const {
        return Cuaternion(
            W * q.W - X * q.X - Y * q.Y - Z * q.Z,
            W * q.X + X * q.W + Y * q.Z - Z * q.Y,
            W * q.Y - X * q.Z + Y * q.W + Z * q.X,
            W * q.Z + X * q.Y - Y * q.X + Z * q.W
        );
    }

    // Obtección de magnitud y normalización
    float ObtenerMagnitud() const {
        return std::sqrt(W * W + X * X + Y * Y + Z * Z);
    }

    Cuaternion ObtenerNormalizado() const {
        float mag = ObtenerMagnitud();
        if (mag > 0.0f) {
            return Cuaternion(W / mag, X / mag, Y / mag, Z / mag);
        }
        return Cuaternion();
    }
};
