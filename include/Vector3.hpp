#pragma once
#include <cmath>
#include <iostream>

struct Vector3 {
    float X;
    float Y;
    float Z;

    // Constructores
    Vector3() : X(0.0f), Y(0.0f), Z(0.0f) {}
    Vector3(float x, float y, float z) : X(x), Y(y), Z(z) {}

    // Operaciones aritméticas básicas
    Vector3 operator+(const Vector3& otro) const {
        return Vector3(X + otro.X, Y + otro.Y, Z + otro.Z);
    }

    Vector3 operator-(const Vector3& otro) const {
        return Vector3(X - otro.X, Y - otro.Y, Z - otro.Z);
    }

    Vector3 operator*(float escalar) const {
        return Vector3(X * escalar, Y * escalar, Z * escalar);
    }

    Vector3 operator/(float escalar) const {
        if (escalar != 0.0f) {
            return Vector3(X / escalar, Y / escalar, Z / escalar);
        }
        return Vector3(0.0f, 0.0f, 0.0f);
    }

    // Operadores de asignación compuesta
    Vector3& operator+=(const Vector3& otro) {
        X += otro.X;
        Y += otro.Y;
        Z += otro.Z;
        return *this;
    }

    Vector3& operator-=(const Vector3& otro) {
        X -= otro.X;
        Y -= otro.Y;
        Z -= otro.Z;
        return *this;
    }

    Vector3& operator*=(float escalar) {
        X *= escalar;
        Y *= escalar;
        Z *= escalar;
        return *this;
    }

    Vector3& operator/=(float escalar) {
        if (escalar != 0.0f) {
            X /= escalar;
            Y /= escalar;
            Z /= escalar;
        } else {
            X = Y = Z = 0.0f;
        }
        return *this;
    }

    // Operadores de comparación
    bool operator==(const Vector3& otro) const {
        return (X == otro.X && Y == otro.Y && Z == otro.Z);
    }

    bool operator!=(const Vector3& otro) const {
        return !(*this == otro);
    }

    // Operador unario menos (Invertir vector)
    Vector3 operator-() const {
        return Vector3(-X, -Y, -Z);
    }

    // Producto Escalar (Dot Product)
    float ProductoEscalar(const Vector3& otro) const {
        return X * otro.X + Y * otro.Y + Z * otro.Z;
    }

    // Producto Cruz / Vectorial (Cross Product)
    Vector3 ProductoCruz(const Vector3& otro) const {
        return Vector3(
            Y * otro.Z - Z * otro.Y,
            Z * otro.X - X * otro.Z,
            X * otro.Y - Y * otro.X
        );
    }

    // ==========================================
    // TENSORS Y FUNCIONES MATEMÁTICAS AVANZADAS
    // ==========================================

    // Producto Tensorial / Externo (Outer Product / Diadic Product: v ⊗ w)
    void ProductoTensorial(const Vector3& otro, float resultado[3][3]) const {
        resultado[0][0] = X * otro.X; resultado[0][1] = X * otro.Y; resultado[0][2] = X * otro.Z;
        resultado[1][0] = Y * otro.X; resultado[1][1] = Y * otro.Y; resultado[1][2] = Y * otro.Z;
        resultado[2][0] = Z * otro.X; resultado[2][1] = Z * otro.Y; resultado[2][2] = Z * otro.Z;
    }

    // Contracción tensorial básica con un vector
    float Contraccion(const Vector3& otro) const {
        return (X * otro.X) + (Y * otro.Y) + (Z * otro.Z);
    }

    // Reflexión de un vector respecto a una normal (Útil para rebotes de física / raycasting)
    Vector3 Reflejar(const Vector3& normal) const {
        return *this - normal * (2.0f * this->ProductoEscalar(normal));
    }

    // Interpolación lineal (Lerp) entre vectores
    Vector3 Interpolar(const Vector3& destino, float t) const {
        return *this + (destino - *this) * t;
    }

    // Longitud y Normalización
    float ObtenerLongitud() const {
        return std::sqrt(X * X + Y * Y + Z * Z);
    }

    Vector3 ObtenerNormalizado() const {
        float longitud = ObtenerLongitud();
        if (longitud > 0.0f) {
            return *this / longitud;
        }
        return Vector3(0.0f, 0.0f, 0.0f);
    }

    void Imprimir() const {
        std::cout << "Vector3(" << X << ", " << Y << ", " << Z << ")\n";
    }
};
