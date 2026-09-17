#pragma once
#include "ContextoGrafico.hpp"
#include <string>
#include <chrono>

struct PropiedadesVentana {
    std::string Titulo;
    unsigned int Ancho;
    unsigned int Alto;

    PropiedadesVentana(const std::string& titulo = "Núcleo del Motor",
                       unsigned int ancho = 1280,
                       unsigned int alto = 720)
        : Titulo(titulo), Ancho(ancho), Alto(alto) {}
};

class Ventana {
public:
    virtual ~Ventana() = default;

    virtual void AlActualizar() = 0;
    virtual unsigned int ObtenerAncho() const = 0;
    virtual unsigned int ObtenerAlto() const = 0;
    virtual bool EstaEjecutandose() const = 0;
    virtual ContextoGrafico* ObtenerContextoGrafico() = 0;

    // Método de creación según la plataforma
    static Ventana* Crear(const PropiedadesVentana& propiedades = PropiedadesVentana());
};
