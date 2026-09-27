#pragma once
#if defined(__linux__)
#include "Ventana.hpp"
#include "ContextoOpenGLLinux.hpp"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>

class VentanaX11 : public Ventana {
private:
    Display* m_PantallaServidor;
    ::Window m_VentanaID;
    Atom m_MensajeEliminar;
    PropiedadesVentana m_Datos;
    bool m_EstaEjecutandose;
    ContextoGrafico* m_ContextoGrafico;

public:
    VentanaX11(const PropiedadesVentana& propiedades) 
        : m_PantallaServidor(nullptr), m_VentanaID(0), m_MensajeEliminar(0), m_Datos(propiedades), m_EstaEjecutandose(true), m_ContextoGrafico(nullptr) {
        
        m_PantallaServidor = XOpenDisplay(NULL);
        if (!m_PantallaServidor) {
            std::cerr << "[VentanaX11] Error crítico: No se pudo abrir la pantalla de X11." << std::endl;
            m_EstaEjecutandose = false;
            return;
        }

        int pantalla = DefaultScreen(m_PantallaServidor);
        ::Window raiz = RootWindow(m_PantallaServidor, pantalla);

        m_VentanaID = XCreateSimpleWindow(
            m_PantallaServidor, raiz, 10, 10,
            m_Datos.Ancho, m_Datos.Alto, 1,
            BlackPixel(m_PantallaServidor, pantalla),
            WhitePixel(m_PantallaServidor, pantalla)
        );

        if (!m_VentanaID) {
            std::cerr << "[VentanaX11] Error crítico: No se pudo crear la ventana simple en X11." << std::endl;
            XCloseDisplay(m_PantallaServidor);
            m_PantallaServidor = nullptr;
            m_EstaEjecutandose = false;
            return;
        }

        // Configuración del título y eventos de entrada de la ventana
        XStoreName(m_PantallaServidor, m_VentanaID, m_Datos.Titulo.c_str());
        XSelectInput(m_PantallaServidor, m_VentanaID, ExposureMask | KeyPressMask | StructureNotifyMask);
        XMapWindow(m_PantallaServidor, m_VentanaID);

        // Habilitar la intercepción del botón de cierre de la ventana (la 'X')
        m_MensajeEliminar = XInternAtom(m_PantallaServidor, "WM_DELETE_WINDOW", False);
        XSetWMProtocols(m_PantallaServidor, m_VentanaID, &m_MensajeEliminar, 1);

        // Inicialización segura del Contexto Gráfico de Linux (GLX)
        m_ContextoGrafico = new ContextoOpenGLLinux(m_PantallaServidor, m_VentanaID);
        if (!m_ContextoGrafico->Inicializar()) {
            std::cerr << "[VentanaX11] Error crítico: Falló la inicialización del contexto OpenGL en Linux." << std::endl;
        }
    }

    ~VentanaX11() override {
        // 1. Primero se libera el contexto gráfico de OpenGL de manera limpia
        if (m_ContextoGrafico) {
            delete m_ContextoGrafico;
            m_ContextoGrafico = nullptr;
        }

        // 2. Después se destruye la ventana de X11 y se cierra la conexión con el servidor gráfico
        if (m_PantallaServidor) {
            if (m_VentanaID) {
                XDestroyWindow(m_PantallaServidor, m_VentanaID);
                m_VentanaID = 0;
            }
            XCloseDisplay(m_PantallaServidor);
            m_PantallaServidor = nullptr;
        }

        std::cout << "[VentanaX11] Ventana X11 y recursos cerrados correctamente." << std::endl;
    }

    void AlActualizar() override {
        if (!m_PantallaServidor) return;

        while (XPending(m_PantallaServidor) > 0) {
            XEvent evento;
            XNextEvent(m_PantallaServidor, &evento);

            if (evento.type == ClientMessage) {
                if (static_cast<Atom>(evento.xclient.data.l[0]) == m_MensajeEliminar) {
                    m_EstaEjecutandose = false;
                }
            } else if (evento.type == ConfigureNotify) {
                m_Datos.Ancho = evento.xconfigure.width;
                m_Datos.Alto = evento.xconfigure.height;
            }
        }
    }

    unsigned int ObtenerAncho() const override { return m_Datos.Ancho; }
    unsigned int ObtenerAlto() const override { return m_Datos.Alto; }
    bool EstaEjecutandose() const override { return m_EstaEjecutandose; }
    ContextoGrafico* ObtenerContextoGrafico() override { return m_ContextoGrafico; }
};

Ventana* Ventana::Crear(const PropiedadesVentana& propiedades) {
    return new VentanaX11(propiedades);
}
#endif
