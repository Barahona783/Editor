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

    // ==========================================
    // NUEVAS FUNCIONES TENSORIALES Y DE PUENTE
    // ==========================================

    // Obtener el cuaternión conjugado (útil para inversiones espaciales)
    Cuaternion ObtenerConjugado() const {
        return Cuaternion(W, -X, -Y, -Z);
    }

    // Convertir el cuaternión a un Tensor de orden 2 (Representación matricial 3x3 compatible)
    void A_Tensor3x3(float tensor[3][3]) const {
        float xx = X * X;
        float xy = X * Y;
        float xz = X * Z;
        float wx = W * X;
        float yy = Y * Y;
        float yz = Y * Z;
        float wy = W * Y;
        float zz = Z * Z;
        float wz = W * Z;

        tensor[0][0] = 1.0f - 2.0f * (yy + zz);
        tensor[0][1] = 2.0f * (xy - wz);
        tensor[0][2] = 2.0f * (xz + wy);

        tensor[1][0] = 2.0f * (xy + wz);
        tensor[1][1] = 1.0f - 2.0f * (xx + zz);
        tensor[1][2] = 2.0f * (yz - wx);

        tensor[2][0] = 2.0f * (xz - wy);
        tensor[2][1] = 2.0f * (yz + wx);
        tensor[2][2] = 1.0f - 2.0f * (xx + yy);
    }
};
