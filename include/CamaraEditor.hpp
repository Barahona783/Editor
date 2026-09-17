#pragma once
#include "Vector3.hpp"
#include "Matriz4x4.hpp"
#include "Entrada.hpp"

class CamaraEditor {
public:
    Vector3 Posicion;
    Vector3 Objetivo;
    float VelocidadNavegacion;

    CamaraEditor(Vector3 posicion = Vector3(0.0f, 3.0f, 8.0f))
        : Posicion(posicion), Objetivo(Vector3(0.0f, 0.0f, 0.0f)), VelocidadNavegacion(4.0f) {}

    void Actualizar(float tiempoDelta) {
        float paso = VelocidadNavegacion * tiempoDelta;

        if (Entrada::EstaTeclaPresionada(CodigoTecla::W)) Posicion.Z -= paso;
        if (Entrada::EstaTeclaPresionada(CodigoTecla::S)) Posicion.Z += paso;
        if (Entrada::EstaTeclaPresionada(CodigoTecla::A)) Posicion.X -= paso;
        if (Entrada::EstaTeclaPresionada(CodigoTecla::D)) Posicion.X += paso;
        if (Entrada::EstaTeclaPresionada(CodigoTecla::Espacio)) Posicion.Y += paso;
    }

    Matriz4x4 ObtenerVistaMatriz() const {
        return Matriz4x4::CrearTraduccion(Vector3(-Posicion.X, -Posicion.Y, -Posicion.Z));
    }

    Matriz4x4 ObtenerProyeccionMatriz(float fov, float aspecto, float cerca, float lejos) const {
        return Matriz4x4::CrearPerspectiva(fov, aspecto, cerca, lejos);
    }
};
