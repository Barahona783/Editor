#pragma once
#include <GL/gl.h>

class RejillaSuelo {
public:
    static void Dibujar(float tamano = 20.0f, float paso = 1.0f) {
        glPushMatrix();
        glLineWidth(1.0f);
        glColor4f(0.25f, 0.28f, 0.35f, 0.5f);

        glBegin(GL_LINES);
        for (float i = -tamano; i <= tamano; i += paso) {
            // Líneas paralelas al eje Z
            glVertex3f(i, 0.0f, -tamano);
            glVertex3f(i, 0.0f, tamano);

            // Líneas paralelas al eje X
            glVertex3f(-tamano, 0.0f, i);
            glVertex3f(tamano, 0.0f, i);
        }
        glEnd();

        glPopMatrix();
    }
};
