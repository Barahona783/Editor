#if defined(__linux__)
#include "ContextoGrafico.hpp"
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <iostream>

class ContextoOpenGLLinux : public ContextoGrafico {
private:
    Display* m_PantallaServidor;
    Window m_VentanaID;
    GLXContext m_ContextoGLX;

public:
    ContextoOpenGLLinux(Display* pantallaServidor, Window ventanaID)
        : m_PantallaServidor(pantallaServidor), m_VentanaID(ventanaID), m_ContextoGLX(nullptr) {}

    ~ContextoOpenGLLinux() override {
        if (m_PantallaServidor && m_ContextoGLX) {
            glXMakeCurrent(m_PantallaServidor, None, nullptr);
            glXDestroyContext(m_PantallaServidor, m_ContextoGLX);
        }
    }

    bool Inicializar() override {
        int atributos[] = {
            GLX_RGBA,
            GLX_DOUBLEBUFFER,
            GLX_DEPTH_SIZE, 24,
            None
        };

        XVisualInfo* visual = glXChooseVisual(m_PantallaServidor, 0, atributos);
        if (!visual) return false;

        m_ContextoGLX = glXCreateContext(m_PantallaServidor, visual, nullptr, GL_TRUE);
        glXMakeCurrent(m_PantallaServidor, m_VentanaID, m_ContextoGLX);
        XFree(visual);

        return true;
    }

    void IntercambiarBúferes() override {
        glXSwapBuffers(m_PantallaServidor, m_VentanaID);
    }

    void EstableserColorLimpieza(float rojo, float verde, float azul, float alfa) override {
        glClearColor(rojo, verde, azul, alfa);
    }

    void LimpiarPantalla() override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
};
#endif
