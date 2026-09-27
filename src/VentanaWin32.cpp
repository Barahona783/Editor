#pragma once
#if defined(_WIN32) || defined(_WIN64)
#include "Ventana.hpp"
#include "ContextoOpenGLWin32.hpp"
#include <windows.h>
#include <iostream>

class VentanaWin32 : public Ventana {
private:
    HWND m_ManejadorVentana;
    HINSTANCE m_Instancia;
    PropiedadesVentana m_Datos;
    bool m_EstaEjecutandose;
    ContextoGrafico* m_ContextoGrafico;

    static LRESULT CALLBACK ProcedimientoVentana(HWND manejador, UINT mensaje, WPARAM parametroW, LPARAM parametroL) {
        VentanaWin32* ventana = nullptr;

        if (mensaje == WM_NCCREATE) {
            // Asignación segura del puntero de la clase nativa desde la creación misma de la ventana Win32
            CREATESTRUCT* estructuraCreacion = reinterpret_cast<CREATESTRUCT*>(parametroL);
            ventana = reinterpret_cast<VentanaWin32*>(estructuraCreacion->lpCreateParams);
            SetWindowLongPtr(manejador, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(ventana));
        } else {
            ventana = reinterpret_cast<VentanaWin32*>(GetWindowLongPtr(manejador, GWLP_USERDATA));
        }

        if (ventana) {
            switch (mensaje) {
                case WM_CLOSE:
                case WM_DESTROY:
                    ventana->m_EstaEjecutandose = false;
                    PostQuitMessage(0);
                    return 0;
                case WM_SIZE:
                    ventana->m_Datos.Ancho = LOWORD(parametroL);
                    ventana->m_Datos.Alto = HIWORD(parametroL);
                    return 0;
            }
        }

        return DefWindowProc(manejador, mensaje, parametroW, parametroL);
    }

public:
    VentanaWin32(const PropiedadesVentana& propiedades) 
        : m_ManejadorVentana(NULL), m_Instancia(NULL), m_Datos(propiedades), m_EstaEjecutandose(true), m_ContextoGrafico(nullptr) {
        
        m_Instancia = GetModuleHandle(NULL);

        WNDCLASSEXW claseVentana = { 0 };
        claseVentana.cbSize = sizeof(WNDCLASSEXW);
        claseVentana.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        claseVentana.lpfnWndProc = ProcedimientoVentana;
        claseVentana.hInstance = m_Instancia;
        claseVentana.hCursor = LoadCursor(NULL, IDC_ARROW);
        claseVentana.lpszClassName = L"ClaseVentanaMotorPersonalizada";

        // Registramos la clase de ventana (si ya está registrada por otra instancia, no es un error crítico)
        RegisterClassExW(&claseVentana);

        RECT rectangulo = { 0, 0, (LONG)m_Datos.Ancho, (LONG)m_Datos.Alto };
        AdjustWindowRect(&rectangulo, WS_OVERLAPPEDWINDOW, FALSE);

        wchar_t tituloTextoAncho[256];
        MultiByteToWideChar(CP_UTF8, 0, m_Datos.Titulo.c_str(), -1, tituloTextoAncho, 256);

        // Pasamos 'this' como parámetro en CreateWindowEx para capturarlo de forma segura en WM_NCCREATE
        m_ManejadorVentana = CreateWindowExW(
            0, claseVentana.lpszClassName, tituloTextoAncho,
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT, CW_USEDEFAULT,
            rectangulo.right - rectangulo.left, rectangulo.bottom - rectangulo.top,
            NULL, NULL, m_Instancia, this
        );

        if (!m_ManejadorVentana) {
            std::cerr << "[VentanaWin32] Error crítico: No se pudo crear la ventana Win32." << std::endl;
            m_EstaEjecutandose = false;
            return;
        }

        // Inicialización segura del Contexto Gráfico de Windows
        m_ContextoGrafico = new ContextoOpenGLWin32(m_ManejadorVentana);
        if (!m_ContextoGrafico->Inicializar()) {
            std::cerr << "[VentanaWin32] Error crítico: Falló la inicialización del contexto OpenGL en Windows." << std::endl;
        }
    }

    ~VentanaWin32() override {
        // 1. Primero se libera el contexto gráfico mientras la ventana y el DC siguen activos
        if (m_ContextoGrafico) {
            delete m_ContextoGrafico;
            m_ContextoGrafico = nullptr;
        }

        // 2. Luego se destruye la ventana de Windows de forma segura
        if (m_ManejadorVentana) {
            DestroyWindow(m_ManejadorVentana);
            m_ManejadorVentana = NULL;
        }

        UnregisterClassW(L"ClaseVentanaMotorPersonalizada", m_Instancia);
        std::cout << "[VentanaWin32] Ventana y recursos destruidos correctamente." << std::endl;
    }

    void AlActualizar() override {
        MSG mensaje;
        while (PeekMessageW(&mensaje, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&mensaje);
            DispatchMessageW(&mensaje);
        }
    }

    unsigned int ObtenerAncho() const override { return m_Datos.Ancho; }
    unsigned int ObtenerAlto() const override { return m_Datos.Alto; }
    bool EstaEjecutandose() const override { return m_EstaEjecutandose; }
    ContextoGrafico* ObtenerContextoGrafico() override { return m_ContextoGrafico; }
};

Ventana* Ventana::Crear(const PropiedadesVentana& propiedades) {
    return new VentanaWin32(propiedades);
}
#endif
