#pragma once
#include "Vector3.hpp"
#include "Matriz4x4.hpp"

class Camara {
private:
    Vector3 m_Posicion;
    Vector3 m_Frente;
    Vector3 m_Arriba;
    float m_CampoDeVision;
    float m_RelacionAspecto;
    float m_Cerca;
    float m_Lejos;

public:
    Camara(Vector3 posicion = Vector3(0.0f, 0.0f, 3.0f),
           float campoDeVision = 45.0f,
           float relacionAspecto = 16.0f / 9.0f,
           float cerca = 0.1f,
           float lejos = 100.0f)
        : m_Posicion(posicion),
          m_Frente(Vector3(0.0f, 0.0f, -1.0f)),
          m_Arriba(Vector3(0.0f, 1.0f, 0.0f)),
          m_CampoDeVision(campoDeVision),
          m_RelacionAspecto(relacionAspecto),
          m_Cerca(cerca),
          m_Lejos(lejos) {}

    Vector3 ObtenerPosicion() const { return m_Posicion; }
    void EstablecerPosicion(const Vector3& posicion) { m_Posicion = posicion; }

    Matriz4x4 ObtenerMatrizVista() const {
        // Genera la transformación inversa de la posición de la cámara
        return Matriz4x4::CrearTraslacion(Vector3(-m_Posicion.X, -m_Posicion.Y, -m_Posicion.Z));
    }

    Matriz4x4 ObtenerMatrizProyeccion() const {
        Matriz4x4 resultado;
        float tanMitadFov = std::tan((m_CampoDeVision * 0.5f) * (3.14159265f / 180.0f));

        resultado.Elementos[0][0] = 1.0f / (m_RelacionAspecto * tanMitadFov);
        resultado.Elementos[1][1] = 1.0f / tanMitadFov;
        resultado.Elementos[2][2] = -(m_Lejos + m_Cerca) / (m_Lejos - m_Cerca);
        resultado.Elementos[2][3] = -(2.0f * m_Lejos * m_Cerca) / (m_Lejos - m_Cerca);
        resultado.Elementos[3][2] = -1.0f;
        resultado.Elementos[3][3] = 0.0f;

        return resultado;
    }
};
