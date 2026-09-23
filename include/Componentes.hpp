#pragma once
#include "../include/Vector3.hpp"
#include "../include/Cuaternion.hpp"
#include "../include/Malla.hpp"
#include "../include/Textura.hpp"
#include "CajaColision.hpp"
#include "RedNeuronal.hpp"          // Subsistema de red neuronal
#include "SistemaMundoAbierto.hpp"   // Subsistema de mundo abierto
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

// --- Componente para manejar scripts y comportamientos personalizados en C++ ---
struct ComponenteScript {
    class ScriptEntidad* Instancia = nullptr;

    template <typename T>
    void Asignar() {
        if (Instancia) delete Instancia;
        Instancia = new T();
    }

    ~ComponenteScript() {
        if (Instancia) {
            delete Instancia;
            Instancia = nullptr;
        }
    }
};

// ==========================================
// COMPONENTE TENSORIAL PARA FÍSICA AVANZADA
// ==========================================

// Componente para almacenar un tensor de esfuerzos o deformaciones de orden 2 (3x3)
// Util para simulaciones de impacto, flexión o deformación estructural de objetos en el motor.
struct ComponenteTensorDeformacion {
    float TensorEsfuerzo[3][3];
    float LimiteElasticidad;
    bool EstaDeformado;

    ComponenteTensorDeformacion(float limiteElasticidad = 100.0f) 
        : LimiteElasticidad(limiteElasticidad), EstaDeformado(false) {
        // Inicializar el tensor de esfuerzo como matriz nula
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                TensorEsfuerzo[i][j] = 0.0f;
            }
        }
    }

    // Actualizar el tensor de esfuerzo a partir del producto tensorial de una fuerza de impacto y una normal
    void AplicarImpactoTensorial(const Vector3& fuerza, const Vector3& normal) {
        fuerza.ProductoTensorial(normal, TensorEsfuerzo);
        
        // Evaluar traza o magnitud simple para comprobar si supera el límite de elasticidad
        float traza = TensorEsfuerzo[0][0] + TensorEsfuerzo[1][1] + TensorEsfuerzo[2][2];
        if (std::abs(traza) > LimiteElasticidad) {
            EstaDeformado = true;
        }
    }
};

// ==========================================
// NUEVOS COMPONENTES DE FÍSICA AVANZADA A LARGO PLAZO
// ==========================================

// Tensor de Inercia Rotacional (Matriz 3x3) para simulación de momentos angulares complejos
struct ComponenteInerciaRotacional {
    float TensorInercia[3][3];
    Vector3 VelocidadAngular;

    ComponenteInerciaRotacional() : VelocidadAngular(0.0f, 0.0f, 0.0f) {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                TensorInercia[i][j] = (i == j) ? 1.0f : 0.0f;
            }
        }
    }
};

// Componente de Restricción Física (Junta / Articulación entre dos entidades)
struct ComponenteRestriccionFisica {
    unsigned int EntidadA;
    unsigned int EntidadB;
    float DistanciaRestitucion;

    ComponenteRestriccionFisica(unsigned int a = 0, unsigned int b = 0, float dist = 1.5f)
        : EntidadA(a), EntidadB(b), DistanciaRestitucion(dist) {}
};

// ==========================================
// COMPONENTE AERODINÁMICO UNIVERSAL (NAVIER-STOKES SIMPLIFICADO)
// ==========================================

// Componente para simular fuerzas de arrastre (drag) y sustentación (lift/downforce) frente al viento ambiental
struct ComponenteAerodinamico {
    float CoeficienteArrastre;       // Resistencia frontal al viento (ej. 0.30 autos F1, 0.8 cajas)
    float AreaFrontal;              // Superficie de impacto del viento en metros cuadrados
    float CoeficienteSustentacion;  // Fuerza vertical (positivo para alas de avión, negativo para downforce en vehículos)
    bool Activo;

    ComponenteAerodinamico(float drag = 0.35f, float area = 2.0f, float lift = -0.5f)
        : CoeficienteArrastre(drag), 
          AreaFrontal(area), 
          CoeficienteSustentacion(lift), 
          Activo(true) {}
};
