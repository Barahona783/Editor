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
#include <vector>

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
struct ComponenteTensorDeformacion {
    float TensorEsfuerzo[3][3];
    float LimiteElasticidad;
    bool EstaDeformado;

    ComponenteTensorDeformacion(float limiteElasticidad = 100.0f) 
        : LimiteElasticidad(limiteElasticidad), EstaDeformado(false) {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                TensorEsfuerzo[i][j] = 0.0f;
            }
        }
    }

    void AplicarImpactoTensorial(const Vector3& fuerza, const Vector3& normal) {
        fuerza.ProductoTensorial(normal, TensorEsfuerzo);
        float traza = TensorEsfuerzo[0][0] + TensorEsfuerzo[1][1] + TensorEsfuerzo[2][2];
        if (std::abs(traza) > LimiteElasticidad) {
            EstaDeformado = true;
        }
    }
};

// ==========================================
// NUEVOS COMPONENTES DE FÍSICA AVANZADA A LARGO PLAZO
// ==========================================
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

struct ComponenteRestriccionFisica {
    unsigned int EntidadA;
    unsigned int EntidadB;
    float DistanciaRestitucion;

    ComponenteRestriccionFisica(unsigned int a = 0, unsigned int b = 0, float dist = 1.5f)
        : EntidadA(a), EntidadB(b), DistanciaRestitucion(dist) {}
};

// ==========================================
// COMPONENTE AERODINÁMICO UNIVERSAL
// ==========================================
struct ComponenteAerodinamico {
    float CoeficienteArrastre;
    float AreaFrontal;
    float CoeficienteSustentacion;
    bool Activo;

    ComponenteAerodinamico(float drag = 0.35f, float area = 2.0f, float lift = -0.5f)
        : CoeficienteArrastre(drag), 
          AreaFrontal(area), 
          CoeficienteSustentacion(lift), 
          Activo(true) {}
};

// ==========================================
// COMPONENTE ÓPTICO (LEY DE FERMAT Y REFRACCIÓN)
// ==========================================
struct ComponenteOptico {
    float IndiceRefraction;
    float RadioInfluencia;
    bool Activo;

    ComponenteOptico(float n = 1.33f, float radio = 10.0f, bool activo = true)
        : IndiceRefraction(n), RadioInfluencia(radio), Activo(activo) {}
};

// ==========================================
// NUEVO: COMPONENTE DE CONSTRUCCIÓN DE TERRENO (MUNDO ABIERTO)
// ==========================================
struct ComponenteTerreno {
    int AnchoMalla;          // Número de vértices en el eje X
    int AltoMalla;           // Número de vértices en el eje Z
    float EscalaHorizontal;  // Separación espacial entre vértices
    float AlturaMaxima;      // Multiplicador de relieve vertical
    std::string RutaAlturaMap; // Archivo RAW o textura de alturas opcional
    std::vector<float> Alturas; // Mapa de alturas en memoria para modificaciones dinámicas

    ComponenteTerreno(int ancho = 64, int alto = 64, float escalaH = 2.0f, float alturaMax = 25.0f)
        : AnchoMalla(ancho),
          AltoMalla(alto),
          EscalaHorizontal(escalaH),
          AlturaMaxima(alturaMax),
          RutaAlturaMap("") {
        // Inicializar la cuadrícula de alturas plana por defecto
        Alturas.resize(ancho * alto, 0.0f);
    }

    // Función para modificar la altura de un punto del terreno (útil para edición o deformación en tiempo real)
    void ModificarAltura(int x, int z, mouseYDelta float deltaAltura) {
        if (x >= 0 && x < AnchoMalla && z >= 0 && z < AltoMalla) {
            Alturas[z * AnchoMalla + x] += deltaAltura;
            if (Alturas[z * AnchoMalla + x] > AlturaMaxima) {
                Alturas[z * AnchoMalla + x] = AlturaMaxima;
            }
        }
    }
};

// ==========================================
// COMPONENTE DE ANIMACIÓN (KEYFRAMES, STATE MACHINES Y BLEND SPACES)
// ==========================================
struct KeyframeTransicion {
    float Tiempo;
    Vector3 Posicion;
    Cuaternion Rotacion;
};

struct ClipAnimacion {
    std::string Nombre;
    float Duracion;
    std::vector<KeyframeTransicion> Keyframes;
};

struct ComponenteAnimacion {
    std::string NombreClip;
    float Duracion;
    float TiempoActual;
    bool EnReproduccion;
    bool EnBucle;

    std::string EstadoActual;
    std::string SiguienteEstado;
    float PesoBlend;
    float VelocidadMezcla;

    ComponenteAnimacion(const std::string& clip = "Idle", float duracion = 2.0f)
        : NombreClip(clip),
          Duracion(duracion),
          TiempoActual(0.0f),
          EnReproduccion(true),
          EnBucle(true),
          EstadoActual("Idle"),
          SiguienteEstado(""),
          PesoBlend(0.0f),
          VelocidadMezcla(5.0f) {}

    ComponenteAnimacion(const std::string& clip, float duracion, bool enBucle)
        : NombreClip(clip),
          Duracion(duracion),
          TiempoActual(0.0f),
          EnReproduccion(true),
          EnBucle(enBucle),
          EstadoActual(clip),
          SiguienteEstado(""),
          PesoBlend(0.0f),
          VelocidadMezcla(5.0f) {}
};
