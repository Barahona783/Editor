#pragma once
#if defined(_WIN32) || defined(_WIN64)
#include "ContextoGrafico.hpp"
#include <windows.h>
#include <GL/gl.h>
#include <iostream>

class ContextoOpenGLWin32 : public ContextoGrafico {
private:
    HWND m_ManejadorVentana;
    HDC m_ContextoDispositivo;
    HGLRC m_ContextoRenderizado;

public:
    ContextoOpenGLWin32(HWND manejadorVentana)
        : m_ManejadorVentana(manejadorVentana), m_ContextoDispositivo(NULL), m_ContextoRenderizado(NULL) {}

    ~ContextoOpenGLWin32() override {
        Liberar();
    }

    bool Inicializar() override {
        if (!m_ManejadorVentana) {
            std::cerr << "[ContextoOpenGLWin32] Error crítico: El manejador de ventana (HWND) es nulo." << std::endl;
            return false;
        }

        // Obtención del Contexto de Dispositivo (DC) de la ventana de Windows
        m_ContextoDispositivo = GetDC(m_ManejadorVentana);
        if (!m_ContextoDispositivo) {
            std::cerr << "[ContextoOpenGLWin32] Error: No se pudo obtener el Device Context (GetDC)." << std::endl;
            return false;
        }

        // Configuración del descriptor de formato de píxeles (RGBA, Doble Búfer, 24 bits de profundidad, 8 bits de stencil)
        PIXELFORMATDESCRIPTOR pfd = { 0 };
        pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 32;
        pfd.cDepthBits = 24;
        pfd.cStencilBits = 8;
        pfd.iLayerType = PFD_MAIN_PLANE;

        int formatoPixel = ChoosePixelFormat(m_ContextoDispositivo, &pfd);
        if (formatoPixel == 0) {
            std::cerr << "[ContextoOpenGLWin32] Error: No se pudo encontrar un formato de píxel compatible." << std::endl;
            ReleaseDC(m_ManejadorVentana, m_ContextoDispositivo);
            m_ContextoDispositivo = NULL;
            return false;
        }

        if (!SetPixelFormat(m_ContextoDispositivo, formatoPixel, &pfd)) {
            std::cerr << "[ContextoOpenGLWin32] Error: No se pudo establecer el formato de píxel." << std::endl;
            ReleaseDC(m_ManejadorVentana, m_ContextoDispositivo);
            m_ContextoDispositivo = NULL;
            return false;
        }

        // Creación del Contexto de Renderizado OpenGL (WGL)
        m_ContextoRenderizado = wglCreateContext(m_ContextoDispositivo);
        if (!m_ContextoRenderizado) {
            std::cerr << "[ContextoOpenGLWin32] Error: No se pudo crear el contexto de renderizado WGL." << std::endl;
            ReleaseDC(m_ManejadorVentana, m_ContextoDispositivo);
            m_ContextoDispositivo = NULL;
            return false;
        }

        // Activación del contexto actual para el hilo de renderizado
        if (!wglMakeCurrent(m_ContextoDispositivo, m_ContextoRenderizado)) {
            std::cerr << "[ContextoOpenGLWin32] Error: No se pudo hacer actual el contexto de renderizado WGL." << std::endl;
            wglDeleteContext(m_ContextoRenderizado);
            m_ContextoRenderizado = NULL;
            ReleaseDC(m_ManejadorVentana, m_ContextoDispositivo);
            m_ContextoDispositivo = NULL;
            return false;
        }

        std::cout << "[ContextoOpenGLWin32] Contexto OpenGL para Windows inicializado con éxito." << std::endl;
        return true;
    }

    void IntercambiarBúferes() override {
        if (m_ContextoDispositivo) {
            SwapBuffers(m_ContextoDispositivo);
        }
    }

    void EstableserColorLimpieza(float rojo, float verde, float azul, float alfa) override {
        glClearColor(rojo, verde, azul, alfa);
    }

    void LimpiarPantalla() override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Liberar() {
        wglMakeCurrent(NULL, NULL);

        if (m_ContextoRenderizado) {
            wglDeleteContext(m_ContextoRenderizado);
            m_ContextoRenderizado = NULL;
        }

        if (m_ContextoDispositivo && m_ManejadorVentana) {
            ReleaseDC(m_ManejadorVentana, m_ContextoDispositivo);
            m_ContextoDispositivo = NULL;
        }

        std::cout << "[ContextoOpenGLWin32] Recursos gráficos liberados correctamente." << std::endl;
    }

    HGLRC ObtenerContextoRenderizado() const {
        return m_ContextoRenderizado;
    }
};
#endif
