#if defined(_WIN32) || defined(_WIN64)
#include "Ventana.hpp"
#include "ContextoOpenGLWin32.cpp"
#include <windows.h>

class VentanaWin32 : public Ventana {
private:
    HWND m_ManejadorVentana;
    HINSTANCE m_Instancia;
    PropiedadesVentana m_Datos;
    bool m_EstaEjecutandose;
    ContextoGrafico* m_ContextoGrafico;

    static LRESULT CALLBACK ProcedimientoVentana(HWND manejador, UINT mensaje, WPARAM parametroW, LPARAM parametroL) {
        VentanaWin32* ventana = reinterpret_cast<VentanaWin32*>(GetWindowLongPtr(manejador, GWLP_USERDATA));
        
        switch (mensaje) {
            case WM_CLOSE:
            case WM_DESTROY:
                if (ventana) ventana->m_EstaEjecutandose = false;
                PostQuitMessage(0);
                return 0;
            case WM_SIZE:
                if (ventana) {
                    ventana->m_Datos.Ancho = LOWORD(parametroL);
                    ventana->m_Datos.Alto = HIWORD(parametroL);
                }
                return 0;
        }
        return DefWindowProc(manejador, mensaje, parametroW, parametroL);
    }

public:
    VentanaWin32(const PropiedadesVentana& propiedades) : m_Datos(propiedades), m_EstaEjecutandose(true) {
        m_Instancia = GetModuleHandle(NULL);

        WNDCLASSEXW claseVentana = { 0 };
        claseVentana.cbSize = sizeof(WNDCLASSEXW);
        claseVentana.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        claseVentana.lpfnWndProc = ProcedimientoVentana;
        claseVentana.hInstance = m_Instancia;
        claseVentana.hCursor = LoadCursor(NULL, IDC_ARROW);
        claseVentana.lpszClassName = L"ClaseVentanaMotorPersonalizada";

        RegisterClassExW(&claseVentana);

        RECT rectangulo = { 0, 0, (LONG)m_Datos.Ancho, (LONG)m_Datos.Alto };
        AdjustWindowRect(&rectangulo, WS_OVERLAPPEDWINDOW, FALSE);

        wchar_t tituloTextoAncho[256];
        MultiByteToWideChar(CP_UTF8, 0, m_Datos.Titulo.c_str(), -1, tituloTextoAncho, 256);

        m_ManejadorVentana = CreateWindowExW(
            0, claseVentana.lpszClassName, tituloTextoAncho,
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT, CW_USEDEFAULT,
            rectangulo.right - rectangulo.left, rectangulo.bottom - rectangulo.top,
            NULL, NULL, m_Instancia, NULL
        );

        SetWindowLongPtr(m_ManejadorVentana, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

        m_ContextoGrafico = new ContextoOpenGLWin32(m_ManejadorVentana);
        m_ContextoGrafico->Inicializar();
    }

    ~VentanaWin32() override {
        delete m_ContextoGrafico;
        DestroyWindow(m_ManejadorVentana);
        UnregisterClassW(L"ClaseVentanaMotorPersonalizada", m_Instancia);
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
