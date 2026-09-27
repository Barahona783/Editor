#pragma once
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
        Liberar();
    }

    bool Inicializar() override {
        if (!m_PantallaServidor) {
            std::cerr << "[ContextoOpenGLLinux] Error crítico: El servidor de pantalla X11 es nulo." << std::endl;
            return false;
        }

        // Atributos de configuración gráfica del búfer para el motor (Color RGBA, Doble Búfer, Profundidad y Stencil)
        int atributos[] = {
            GLX_RGBA,
            GLX_DOUBLEBUFFER,
            GLX_DEPTH_SIZE, 24,
            GLX_STENCIL_SIZE, 8,
            None
        };

        XVisualInfo* visual = glXChooseVisual(m_PantallaServidor, 0, atributos);
        if (!visual) {
            std::cerr << "[ContextoOpenGLLinux] Error: No se pudo encontrar un Visual compatible con GLX en X11." << std::endl;
            return false;
        }

        // Creación del Contexto OpenGL nativo
        m_ContextoGLX = glXCreateContext(m_PantallaServidor, visual, nullptr, GL_TRUE);
        if (!m_ContextoGLX) {
            std::cerr << "[ContextoOpenGLLinux] Error: No se pudo crear el contexto GLX." << std::endl;
            XFree(visual);
            return false;
        }

        // Vinculación del contexto a la ventana activa
        if (!glXMakeCurrent(m_PantallaServidor, m_VentanaID, m_ContextoGLX)) {
            std::cerr << "[ContextoOpenGLLinux] Error: No se pudo hacer actual el contexto GLX en la ventana." << std::endl;
            glXDestroyContext(m_PantallaServidor, m_ContextoGLX);
            m_ContextoGLX = nullptr;
            XFree(visual);
            return false;
        }

        XFree(visual);
        std::cout << "[ContextoOpenGLLinux] Contexto OpenGL inicializado y vinculado con éxito." << std::endl;
        return true;
    }

    void IntercambiarBúferes() override {
        if (m_PantallaServidor && m_VentanaID) {
            glXSwapBuffers(m_PantallaServidor, m_VentanaID);
        }
    }

    void EstableserColorLimpieza(float rojo, float verde, float azul, float alfa) override {
        glClearColor(rojo, verde, azul, alfa);
    }

    void LimpiarPantalla() override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Liberar() {
        if (m_PantallaServidor && m_ContextoGLX) {
            glXMakeCurrent(m_PantallaServidor, None, nullptr);
            glXDestroyContext(m_PantallaServidor, m_ContextoGLX);
            m_ContextoGLX = nullptr;
            std::cout << "[ContextoOpenGLLinux] Contexto OpenGL liberado correctamente." << std::endl;
        }
    }

    GLXContext ObtenerContextoGLX() const {
        return m_ContextoGLX;
    }
};
#endif
