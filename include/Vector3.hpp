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
