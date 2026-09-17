#pragma once
#include <GL/gl.h>
#include "Vector3.hpp"

struct LuzDireccional {
    Vector3 direccion;
    Vector3 color;
    float intensidad;

    LuzDireccional() 
        : direccion(Vector3(0.0f, -1.0f, -1.0f)), color(Vector3(1.0f, 1.0f, 1.0f)), intensidad(1.0f) {}
};

class SistemaIluminacion {
public:
    static void AplicarLuz(const LuzDireccional& luz) {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);

        GLfloat dir[] = { -luz.direccion.X, -luz.direccion.Y, -luz.direccion.Z, 0.0f };
        GLfloat col[] = { luz.color.X * luz.intensidad, luz.color.Y * luz.intensidad, luz.color.Z * luz.intensidad, 1.0f };

        glLightfv(GL_LIGHT0, GL_POSITION, dir);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, col);
    }

    static void Desactivar() {
        glDisable(GL_LIGHTING);
    }
};
