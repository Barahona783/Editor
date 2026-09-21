#pragma once
#include <GL/gl.h>
#include "Componentes.hpp"

class GizmosEditor {
public:
    enum ModoGizmo {
        TRASLACION,
        ROTACION,
        ESCALA
    };

    ModoGizmo modoActual = TRASLACION;

    // Método principal para renderizar los ejes 3D del gizmo sobre la posición de la entidad
    void RenderizarGizmo3D(const ComponenteTransformacion& transform) {
        glLineWidth(3.0f);
        glBegin(GL_LINES);
            // Eje X (Rojo)
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex3f(transform.Posicion.X, transform.Posicion.Y, transform.Posicion.Z);
            glVertex3f(transform.Posicion.X + 1.5f, transform.Posicion.Y, transform.Posicion.Z);

            // Eje Y (Verde)
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(transform.Posicion.X, transform.Posicion.Y, transform.Posicion.Z);
            glVertex3f(transform.Posicion.X, transform.Posicion.Y + 1.5f, transform.Posicion.Z);

            // Eje Z (Azul)
            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex3f(transform.Posicion.X, transform.Posicion.Y, transform.Posicion.Z);
            glVertex3f(transform.Posicion.X, transform.Posicion.Y, transform.Posicion.Z - 1.5f);
        glEnd();
        glLineWidth(1.0f);
    }

    // Cambiar dinámicamente el modo del gizmo (útil para atajos de teclado o botones de la UI)
    void CambiarModo(ModoGizmo nuevoModo) {
        modoActual = nuevoModo;
    }
};
