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
        wglMakeCurrent(NULL, NULL);
        if (m_ContextoRenderizado) wglDeleteContext(m_ContextoRenderizado);
        if (m_ContextoDispositivo) ReleaseDC(m_ManejadorVentana, m_ContextoDispositivo);
    }

    bool Inicializar() override {
        m_ContextoDispositivo = GetDC(m_ManejadorVentana);

        PIXELFORMATDESCRIPTOR pfd = { 0 };
        pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 32;
        pfd.cDepthBits = 24;
        pfd.cStencilBits = 8;

        int formatoPixel = ChoosePixelFormat(m_ContextoDispositivo, &pfd);
        if (formatoPixel == 0) return false;

        SetPixelFormat(m_ContextoDispositivo, formatoPixel, &pfd);

        m_ContextoRenderizado = wglCreateContext(m_ContextoDispositivo);
        wglMakeCurrent(m_ContextoDispositivo, m_ContextoRenderizado);

        return true;
    }

    void IntercambiarBúferes() override {
        SwapBuffers(m_ContextoDispositivo);
    }

    void EstableserColorLimpieza(float rojo, float verde, float azul, float alfa) override {
        glClearColor(rojo, verde, azul, alfa);
    }

    void LimpiarPantalla() override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
};
#endif
