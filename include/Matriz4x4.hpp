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

    // ==========================================
    // MULTIPLICACIÓN MATRIZ x VECTOR (Esencial para Redes Neuronales Z = WX)
    // ==========================================
    Vector3 operator*(const Vector3& vec) const {
        float x = Elementos[0][0] * vec.X + Elementos[0][1] * vec.Y + Elementos[0][2] * vec.Z + Elementos[0][3];
        float y = Elementos[1][0] * vec.X + Elementos[1][1] * vec.Y + Elementos[1][2] * vec.Z + Elementos[1][3];
        float z = Elementos[2][0] * vec.X + Elementos[2][1] * vec.Y + Elementos[2][2] * vec.Z + Elementos[2][3];
        return Vector3(x, y, z);
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

    // ==========================================
    // FUNCIONES TENSORIALES Y AVANZADAS
    // ==========================================

    // Obtener la matriz transpuesta
    Matriz4x4 ObtenerTranspuesta() const {
        Matriz4x4 resultado;
        for (int fila = 0; fila < 4; ++fila) {
            for (int columna = 0; columna < 4; ++columna) {
                resultado.Elementos[fila][columna] = Elementos[columna][fila];
            }
        }
        return resultado;
    }

    // Calcular la traza (suma de la diagonal principal), útil para invariantes de tensores
    float ObtenerTraza() const {
        return Elementos[0][0] + Elementos[1][1] + Elementos[2][2] + Elementos[3][3];
    }

    // Cargar un tensor de 3x3 (producto tensorial) en la submatriz superior izquierda del motor
    void CargarDesdeTensor3x3(const float tensor[3][3]) {
        CargarIdentidad();
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                Elementos[i][j] = tensor[i][j];
            }
        }
    }

    // ==========================================
    // AMPLIACIONES MATRICIALES (DETERMINANTE E INVERSA)
    // ==========================================

    // Calcular el determinante de la matriz 4x4
    float ObtenerDeterminante() const {
        float SubFactor0 = Elementos[2][2] * Elementos[3][3] - Elementos[2][3] * Elementos[3][2];
        float SubFactor1 = Elementos[2][1] * Elementos[3][3] - Elementos[2][3] * Elementos[3][1];
        float SubFactor2 = Elementos[2][1] * Elementos[3][2] - Elementos[2][2] * Elementos[3][1];
        float SubFactor3 = Elementos[2][0] * Elementos[3][3] - Elementos[2][3] * Elementos[3][0];
        float SubFactor4 = Elementos[2][0] * Elementos[3][2] - Elementos[2][2] * Elementos[3][0];
        float SubFactor5 = Elementos[2][0] * Elementos[3][1] - Elementos[2][1] * Elementos[3][0];

        float det = Elementos[0][0] * (Elementos[1][1] * SubFactor0 - Elementos[1][2] * SubFactor1 + Elementos[1][3] * SubFactor2)
                  - Elementos[0][1] * (Elementos[1][0] * SubFactor0 - Elementos[1][2] * SubFactor3 + Elementos[1][3] * SubFactor4)
                  + Elementos[0][2] * (Elementos[1][0] * SubFactor1 - Elementos[1][1] * SubFactor3 + Elementos[1][3] * SubFactor5)
                  - Elementos[0][3] * (Elementos[1][0] * SubFactor2 - Elementos[1][1] * SubFactor4 + Elementos[1][2] * SubFactor5);
        return det;
    }

    // Obtener la matriz inversa mediante cofactores y adjunta
    Matriz4x4 ObtenerInversa() const {
        Matriz4x4 inversa;
        float det = ObtenerDeterminante();
        if (std::abs(det) < 1e-8f) {
            return *this; // Retorna la matriz original si no es invertible
        }

        float invDet = 1.0f / det;

        const float* m = &Elementos[0][0];
        float* inv = &inversa.Elementos[0][0];

        inv[0] =  (m[5] * (m[10] * m[15] - m[11] * m[14]) - m[9] * (m[6] * m[15] - m[7] * m[14]) + m[13] * (m[6] * m[11] - m[7] * m[10])) * invDet;
        inv[1] = -(m[1] * (m[10] * m[15] - m[11] * m[14]) - m[9] * (m[2] * m[15] - m[3] * m[14]) + m[13] * (m[2] * m[11] - m[3] * m[10])) * invDet;
        inv[2] =  (m[1] * (m[6]  * m[15] - m[7]  * m[14]) - m[5] * (m[2] * m[15] - m[3] * m[14]) + m[13] * (m[2] * m[7]  - m[3]  * m[6]))  * invDet;
        inv[3] = -(m[1] * (m[6]  * m[11] - m[7]  * m[10]) - m[5] * (m[2] * m[11] - m[3] * m[10]) + m[9]  * (m[2] * m[7]  - m[3]  * m[6]))  * invDet;

        inv[4] = -(m[4] * (m[10] * m[15] - m[11] * m[14]) - m[8] * (m[6] * m[15] - m[7] * m[14]) + m[12] * (m[6] * m[11] - m[7] * m[10])) * invDet;
        inv[5] =  (m[0] * (m[10] * m[15] - m[11] * m[14]) - m[8] * (m[2] * m[15] - m[3] * m[14]) + m[12] * (m[2] * m[11] - m[3] * m[10])) * invDet;
        inv[6] = -(m[0] * (m[6]  * m[15] - m[7]  * m[14]) - m[4] * (m[2] * m[15] - m[3] * m[14]) + m[12] * (m[2] * m[7]  - m[3]  * m[6]))  * invDet;
        inv[7] =  (m[0] * (m[6]  * m[11] - m[7]  * m[10]) - m[4] * (m[2] * m[11] - m[3] * m[10]) + m[8]  * (m[2] * m[7]  - m[3]  * m[6]))  * invDet;

        inv[8] =  (m[4] * (m[9]  * m[15] - m[11] * m[13]) - m[8] * (m[5] * m[15] - m[7] * m[13]) + m[12] * (m[5] * m[11] - m[7] * m[9]))  * invDet;
        inv[9] = -(m[0] * (m[9]  * m[15] - m[11] * m[13]) - m[8] * (m[1] * m[15] - m[3] * m[13]) + m[12] * (m[1] * m[11] - m[3] * m[9]))  * invDet;
        inv[10]=  (m[0] * (m[5]  * m[15] - m[7]  * m[13]) - m[4] * (m[1] * m[15] - m[3] * m[13]) + m[12] * (m[1] * m[7]  - m[3]  * m[5]))  * invDet;
        inv[11]= -(m[0] * (m[5]  * m[11] - m[7]  * m[9])  - m[4] * (m[1] * m[11] - m[3] * m[9])  + m[8]  * (m[1] * m[7]  - m[3]  * m[5]))  * invDet;

        inv[12]= -(m[4] * (m[9]  * m[14] - m[10] * m[13]) - m[8] * (m[5] * m[14] - m[6] * m[13]) + m[12] * (m[5] * m[10] - m[6] * m[9]))  * invDet;
        inv[13]=  (m[0] * (m[9]  * m[14] - m[10] * m[13]) - m[8] * (m[1] * m[14] - m[2] * m[13]) + m[12] * (m[1] * m[10] - m[2] * m[9]))  * invDet;
        inv[14]= -(m[0] * (m[5]  * m[14] - m[6]  * m[13]) - m[4] * (m[1] * m[14] - m[2] * m[13]) + m[12] * (m[1] * m[6]  - m[2]  * m[5]))  * invDet;
        inv[15]=  (m[0] * (m[5]  * m[10] - m[6]  * m[9])  - m[4] * (m[1] * m[10] - m[2] * m[9])  + m[8]  * (m[1] * m[6]  - m[2]  * m[5]))  * invDet;

        return inversa;
    }
};
