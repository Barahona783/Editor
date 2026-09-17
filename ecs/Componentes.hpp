#pragma once
#include "Vector3.hpp"
#include "Cuaternion.hpp"
#include "Malla.hpp"
#include "Textura.hpp"
#include "CajaColision.hpp"
#include <string>
#include <memory>

// Componente para manejar la posición, rotación y escala en el mundo 3D
struct ComponenteTransformacion {
    Vector3 Posicion;
    Vector3 Escala;
    Cuaternion Rotacion;

    ComponenteTransformacion(
        Vector3 posicion = Vector3(0.0f, 0.0f, 0.0f),
        Vector3 escala = Vector3(1.0f, 1.0f, 1.0f)
    ) : Posicion(posicion), Escala(escala), Rotacion(Cuaternion()) {}
};

// Componente para identificar y etiquetar la entidad
struct ComponenteNombre {
    std::string Nombre;

    ComponenteNombre(const std::string& nombre = "EntidadSinNombre")
        : Nombre(nombre) {}
};

// Componente para renderizado de geometría 3D
struct ComponenteMalla {
    std::shared_ptr<Malla> MallaObjeto;

    ComponenteMalla(std::shared_ptr<Malla> malla = nullptr)
        : MallaObjeto(malla) {}
};

// Componente para manejar la textura del objeto
struct ComponenteTextura {
    std::shared_ptr<Textura> TexturaObjeto;

    ComponenteTextura(std::shared_ptr<Textura> textura = nullptr)
        : TexturaObjeto(textura) {}
};

// Componente para propiedades físicas y simulación de fuerzas
struct ComponenteCuerpoRigido {
    Vector3 Velocidad;
    float Masa;
    float EscalaGravedad;
    bool EsDinamico;
    bool EnElSuelo;

    ComponenteCuerpoRigido(
        float masa = 1.0f,
        bool esDinamico = true,
        float escalaGravedad = 1.0f
    ) : Velocidad(Vector3(0.0f, 0.0f, 0.0f)),
        Masa(masa),
        EscalaGravedad(escalaGravedad),
        EsDinamico(esDinamico),
        EnElSuelo(false) {}
};

// Componente de límites de colisión 3D
struct ComponenteColisionador {
    CajaColision Caja;

    ComponenteColisionador(CajaColision caja = CajaColision())
        : Caja(caja) {}
};

// Componente para reproducción de sonido
struct ComponenteFuenteAudio {
    std::string RutaArchivo;
    bool EnBucle;
    bool DebeReproducir;

    ComponenteFuenteAudio(
        const std::string& rutaArchivo = "",
        bool enBucle = false
    ) : RutaArchivo(rutaArchivo),
        EnBucle(enBucle),
        DebeReproducir(false) {}
};
