#pragma once
#include <GL/gl.h>
#include "Vector3.hpp"

struct LuzDireccional {
    Vector3 direccion;
    Vector3 color;
    float intensidad;
    Vector3 ambiental; // Componente ambiental para iluminar zonas en sombra en el mundo abierto

    LuzDireccional() 
        : direccion(Vector3(0.0f, -1.0f, -1.0f)), 
          color(Vector3(1.0f, 1.0f, 1.0f)), 
          intensidad(1.0f),
          ambiental(Vector3(0.2f, 0.2f, 0.2f)) {} // Luz ambiental suave por defecto
};

class SistemaIluminacion {
public:
    static void AplicarLuz(const LuzDireccional& luz) {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);

        // Configuración de la dirección de la luz (invertida para el cálculo vectorial de OpenGL)
        GLfloat dir[] = { -luz.direccion.X, -luz.direccion.Y, -luz.direccion.Z, 0.0f };
        
        // Color difuso modulado por la intensidad global de la fuente
        GLfloat col[] = { 
            luz.color.X * luz.intensidad, 
            luz.color.Y * luz.intensidad, 
            luz.color.Z * luz.intensidad, 
            1.0f 
        };

        // Color ambiental para evitar oscuridad absoluta en los sectores no alcanzados directamente por la luz
        GLfloat amb[] = { 
            luz.ambiental.X, 
            luz.ambiental.Y, 
            luz.ambiental.Z, 
            1.0f 
        };

        glLightfv(GL_LIGHT0, GL_POSITION, dir);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, col);
        glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
    }

    static void Desactivar() {
        glDisable(GL_LIGHTING);
    }
};
