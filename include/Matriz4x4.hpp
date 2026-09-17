#pragma once
#include "Vector3.hpp"
#include <cmath>
#include <cstring>

struct Matriz4x4 {
    float Elementos[4][4];

    Matriz4x4() {
        CargarIdentidad();
    }

    void CargarIdentidad() {
        std::memset(Elementos, 0, sizeof(Elementos));
        Elementos[0][0] = 1.0f;
        Elementos[1][1] = 1.0f;
        Elementos[2][2] = 1.0f;
        Elementos[3][3] = 1.0f;
    }

    // Multiplicación de Matrices
    Matriz4x4 operator*(const Matriz4x4& otra) const {
        Matriz4x4 resultado;
        for (int fila = 0; fila < 4; ++fila) {
            for (int columna = 0; columna < 4; ++columna) {
                resultado.Elementos[fila][columna] = 0.0f;
                for (int k = 0; k < 4; ++k) {
                    resultado.Elementos[fila][columna] += Elementos[fila][k] * otra.Elementos[k][columna];
                }
            }
        }
        return resultado;
    }

    // Generadores de Transformación
    static Matriz4x4 CrearTraslacion(const Vector3& posicion) {
        Matriz4x4 resultado;
        resultado.Elementos[0][3] = posicion.X;
        resultado.Elementos[1][3] = posicion.Y;
        resultado.Elementos[2][3] = posicion.Z;
        return resultado;
    }

    static Matriz4x4 CrearEscala(const Vector3& escala) {
        Matriz4x4 resultado;
        resultado.Elementos[0][0] = escala.X;
        resultado.Elementos[1][1] = escala.Y;
        resultado.Elementos[2][2] = escala.Z;
        return resultado;
    }

    static Matriz4x4 CrearRotacionZ(float anguloRadianes) {
        Matriz4x4 resultado;
        float coseno = std::cos(anguloRadianes);
        float seno = std::sin(anguloRadianes);

        resultado.Elementos[0][0] = coseno;
        resultado.Elementos[0][1] = -seno;
        resultado.Elementos[1][0] = seno;
        resultado.Elementos[1][1] = coseno;

        return resultado;
    }

    // Matriz de Proyección en Perspectiva
    static Matriz4x4 CrearProyeccion(float fovGrados, float aspecto, float cerca, float lejos) {
        Matriz4x4 resultado;
        std::memset(resultado.Elementos, 0, sizeof(resultado.Elementos));

        float fovRadianes = fovGrados * (3.14159265359f / 180.0f);
        float tanMitadFov = std::tan(fovRadianes / 2.0f);

        resultado.Elementos[0][0] = 1.0f / (aspecto * tanMitadFov);
        resultado.Elementos[1][1] = 1.0f / tanMitadFov;
        resultado.Elementos[2][2] = -(lejos + cerca) / (lejos - cerca);
        resultado.Elementos[2][3] = -(2.0f * lejos * cerca) / (lejos - cerca);
        resultado.Elementos[3][2] = -1.0f;

        return resultado;
    }
};
