#pragma once
#include <GL/gl.h>
#include "Vector3.hpp"

class Gizmo3D {
public:
    static void DibujarEjes(const Vector3& posicion, float tamano = 1.0f) {
        // Guardar los atributos actuales de color y ancho de línea para no contaminar el estado global de OpenGL
        glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT);

        glPushMatrix();
        glTranslatef(posicion.X, posicion.Y, posicion.Z);
        
        glLineWidth(3.0f);

        glBegin(GL_LINES);
            // Eje X - Rojo
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(tamano, 0.0f, 0.0f);

            // Eje Y - Verde
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.0f, tamano, 0.0f);

            // Eje Z - Azul
            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.0f, 0.0f, tamano);
        glEnd();

        glPopMatrix();

        // Restaurar los estados previos de OpenGL automáticamente
        glPopAttrib();
    }
};
