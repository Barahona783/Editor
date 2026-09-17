#if defined(__linux__)
#include "Ventana.hpp"
#include "ContextoOpenGLLinux.cpp"
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
    VentanaX11(const PropiedadesVentana& propiedades) : m_Datos(propiedades), m_EstaEjecutandose(true) {
        m_PantallaServidor = XOpenDisplay(NULL);
        if (!m_PantallaServidor) {
            std::cerr << "Error al abrir la pantalla de X11\n";
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

        XStoreName(m_PantallaServidor, m_VentanaID, m_Datos.Titulo.c_str());
        XSelectInput(m_PantallaServidor, m_VentanaID, ExposureMask | KeyPressMask | StructureNotifyMask);
        XMapWindow(m_PantallaServidor, m_VentanaID);

        m_MensajeEliminar = XInternAtom(m_PantallaServidor, "WM_DELETE_WINDOW", False);
        XSetWMProtocols(m_PantallaServidor, m_VentanaID, &m_MensajeEliminar, 1);

        m_ContextoGrafico = new ContextoOpenGLLinux(m_PantallaServidor, m_VentanaID);
        m_ContextoGrafico->Inicializar();
    }

    ~VentanaX11() override {
        delete m_ContextoGrafico;
        if (m_PantallaServidor) {
            XDestroyWindow(m_PantallaServidor, m_VentanaID);
            XCloseDisplay(m_PantallaServidor);
        }
    }

    void AlActualizar() override {
        while (XPending(m_PantallaServidor) > 0) {
            XEvent evento;
            XNextEvent(m_PantallaServidor, &evento);

            if (evento.type == ClientMessage) {
                if ((Atom)evento.xclient.data.l[0] == m_MensajeEliminar) {
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
