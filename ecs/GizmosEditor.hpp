#pragma once
#include <GL/gl.h>
#include <cmath>
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

    // Dibujado de cajas magenta y círculos cian de depuración espacial sin alterar lo existente
    void RenderizarDepuracionEspacial(const ComponenteTransformacion& transform, float radioInfluencia = 3.5f) {
        glLineWidth(2.0f);
        
        // 1. Caja Delimitadora (Bounding Box) en Magenta
        glColor3f(1.0f, 0.0f, 1.0f);
        float semiancho = 2.0f;
        float semiprofundidad = 3.0f;
        float ySuelo = transform.Posicion.Y;

        glBegin(GL_LINE_LOOP);
            glVertex3f(transform.Posicion.X - semiancho, ySuelo, transform.Posicion.Z - semiprofundidad);
            glVertex3f(transform.Posicion.X + semiancho, ySuelo, transform.Posicion.Z - semiprofundidad);
            glVertex3f(transform.Posicion.X + semiancho, ySuelo, transform.Posicion.Z + semiprofundidad);
            glVertex3f(transform.Posicion.X - semiancho, ySuelo, transform.Posicion.Z + semiprofundidad);
        glEnd();

        // 2. Radio de Influencia / Percepción en Cian
        glColor3f(0.0f, 1.0f, 1.0f);
        int segmentos = 32;
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < segmentos; ++i) {
            float theta = 2.0f * 3.1415926f * float(i) / float(segmentos);
            float dx = radioInfluencia * cosf(theta);
            float dz = radioInfluencia * sinf(theta);
            glVertex3f(transform.Posicion.X + dx, ySuelo + 0.05f, transform.Posicion.Z + dz);
        }
        glEnd();
        
        glLineWidth(1.0f);
    }

    // Cambiar dinámicamente el modo del gizmo (útil para atajos de teclado o botones de la UI)
    void CambiarModo(ModoGizmo nuevoModo) {
        modoActual = nuevoModo;
    }
};
