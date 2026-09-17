#pragma once
#include <GL/gl.h>
#include "Vector3.hpp"

class Gizmo3D {
public:
    static void DibujarEjes(const Vector3& posicion, float tamano = 1.0f) {
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

        glLineWidth(1.0f);
        glPopMatrix();
    }
};
