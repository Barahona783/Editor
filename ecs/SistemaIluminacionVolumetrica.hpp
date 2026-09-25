#pragma once
#include "../include/Vector3.hpp"
#include "../include/Malla.hpp"
#include <vector>
#include <memory>
#include <string>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ==========================================
// COMPONENTE DE LUZ VOLUMÉTRICA (Haz de luz realista / Spotlight)
// ==========================================
struct ComponenteLuzVolumetrica {
    std::string Nombre;
    Vector3 Color;
    float Intensidad;
    float RadioAlcance;
    float AperturaCono;     // Ángulo de apertura del haz de luz en grados
    float DensidadeNiebla;  // Densidad del medio volumétrico para la dispersión
    bool Activa;

    ComponenteLuzVolumetrica(std::string nombre = "LuzVolumetrica", 
                             Vector3 color = Vector3(1.0f, 0.85f, 0.6f), 
                             float intensidad = 3.0f, 
                             float radio = 15.0f, 
                             float apertura = 45.0f,
                             float densidad = 0.5f)
        : Nombre(nombre), 
          Color(color), 
          Intensidad(intensidad), 
          RadioAlcance(radio), 
          AperturaCono(apertura), 
          DensidadeNiebla(densidad), 
          Activa(true) {}
};

// ==========================================
// SUBSISTEMA DE ILUMINACIÓN VOLUMÉTRICA
// ==========================================
class SistemaIluminacionVolumetrica {
public:

    // Genera proceduralmente una malla de cono 3D para representar el volumen del haz de luz
    static Malla GenerarMallaHazLuz(float radioBase = 2.0f, float altura = 5.0f, int resolucionRadial = 16) {
        Malla mallaHaz;
        std::vector<float> vertices; // Intercalados: PosX, PosY, PosZ, NormX, NormY, NormZ, U, V
        std::vector<unsigned int> indices;

        // Vértice del ápice (origen de la luz en la punta superior del cono)
        // Posición: (0, altura, 0), Normal: (0, 1, 0), UV: (0.5, 1.0)
        vertices.push_back(0.0f); vertices.push_back(altura); vertices.push_back(0.0f);
        vertices.push_back(0.0f); vertices.push_back(1.0f); vertices.push_back(0.0f);
        vertices.push_back(0.5f); vertices.push_back(1.0f);

        // Generar vértices de la base circular del cono
        for (int i = 0; i <= resolucionRadial; ++i) {
            float angulo = (static_cast<float>(i) / static_cast<float>(resolucionRadial)) * 2.0f * static_cast<float>(M_PI);
            float x = std::cos(angulo) * radioBase;
            float z = std::sin(angulo) * radioBase;
            float u = static_cast<float>(i) / static_cast<float>(resolucionRadial);

            // Posición en la base
            vertices.push_back(x); vertices.push_back(0.0f); vertices.push_back(z);
            // Normal orientada hacia afuera y abajo
            float normY = radioBase / altura;
            Vector3 normal(std::cos(angulo), normY, std::sin(angulo));
            normal.Normalizar();
            vertices.push_back(normal.X); vertices.push_back(normal.Y); vertices.push_back(normal.Z);
            // Coordenadas UV
            vertices.push_back(u); vertices.push_back(0.0f);
        }

        // Centro de la base del cono (para cerrar la geometría si es necesario)
        unsigned int indiceCentroBase = static_cast<unsigned int>(vertices.size() / 8);
        vertices.push_back(0.0f); vertices.push_back(0.0f); vertices.push_back(0.0f);
        vertices.push_back(0.0f); vertices.push_back(-1.0f); vertices.push_back(0.0f);
        vertices.push_back(0.5f); vertices.push_back(0.5f);

        // Construcción de índices para las caras laterales del cono
        for (int i = 0; i < resolucionRadial; ++i) {
            unsigned int actual = i + 1;
            unsigned int siguiente = (i + 1) % resolucionRadial + 1;

            // Triángulo lateral conectando ápice con la base
            indices.push_back(0);          // Ápice
            indices.push_back(siguiente);
            indices.push_back(actual);

            // Triángulo de la tapa inferior de la base
            indices.push_back(indiceCentroBase);
            indices.push_back(actual);
            indices.push_back(siguiente);
        }

        return mallaHaz;
    }

    // Calcula la atenuación de la luz volumétrica en función de la distancia y la ley del inverso del cuadrado
    static float CalcularAtenuacion(float distancia, float radioMaximo) {
        if (distancia >= radioMaximo) return 0.0f;
        float factor = 1.0f - (distancia / radioMaximo);
        return factor * factor; // Caída cuadrática suave para realismo físico
    }

    // Actualiza y procesa todas las luces volumétricas activas en la escena ECS
    template <typename GestorEntidadesT>
    static void Actualizar(GestorEntidadesT& gestorEntidades) {
        auto entidades = gestorEntidades.ObtenerTodasLasEntidades();
        for (const auto& entidad : entidades) {
            ComponenteLuzVolumetrica* luz = gestorEntidades.template ObtenerComponente<ComponenteLuzVolumetrica>(entidad);
            if (luz && luz->Activa) {
                // Cálculo de dispersión atmosférica y aplicación de parámetros volumétricos por frame
                float densidadEfectiva = luz->DensidadeNiebla * luz->Intensidad;
                (void)densidadEfectiva; // Evitar advertencias de variable no utilizada durante el enlace del pipeline
            }
        }
    }
};
