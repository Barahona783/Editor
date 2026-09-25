#pragma once
#include "../include/Vector3.hpp"
#include "../include/Malla.hpp"
#include <vector>
#include <memory>
#include <cmath>
#include <string>

// Estructura de vértice extendida para modelado y edición 3D
struct VerticeModelado {
    Vector3 Posicion;
    Vector3 Normal;
    float U, V; // Coordenadas de textura

    VerticeModelado(Vector3 pos = Vector3(0,0,0), Vector3 norm = Vector3(0,1,0), float u = 0.0f, float v = 0.0f)
        : Posicion(pos), Normal(norm), U(u), V(v) {}
};

// Estructura de malla en bruto editable antes de convertirse en un recurso estático de GPU
struct MallaEditable {
    std::string Nombre;
    std::vector<VerticeModelado> Vertices;
    std::vector<unsigned int> Indices;

    // Añadir un triángulo a la malla en construcción
    void AgregarTriangulo(const VerticeModelado& v1, const VerticeModelado& v2, const VerticeModelado& v3) {
        unsigned int baseIndex = static_cast<unsigned int>(Vertices.size());
        Vertices.push_back(v1);
        Vertices.push_back(v2);
        Vertices.push_back(v3);
        
        Indices.push_back(baseIndex);
        Indices.push_back(baseIndex + 1);
        Indices.push_back(baseIndex + 2);
    }

    // Limpiar geometría actual
    void Limpiar() {
        Vertices.clear();
        Indices.clear();
    }
};

// ==========================================
// SUBSISTEMA DE MODELADO 3D PROCEDURAL Y EDITABLE
// ==========================================
class SistemaModelado3D {
public:

    // 1. Crear un cubo editable proceduralmente
    static MallaEditable GenerarCubo(float ancho = 1.0f, float alto = 1.0f, float profundidad = 1.0f) {
        MallaEditable malla;
        malla.Nombre = "CuboProcedural";

        float ha = ancho * 0.5f;
        float hb = alto * 0.5f;
        float hp = profundidad * 0.5f;

        // Definición de vértices básicos por caras para permitir normales correctas
        // Cara Frontal
        VerticeModelado v0(Vector3(-ha, -hb,  hp), Vector3(0, 0, 1), 0.0f, 0.0f);
        VerticeModelado v1(Vector3( ha, -hb,  hp), Vector3(0, 0, 1), 1.0f, 0.0f);
        VerticeModelado v2(Vector3( ha,  hb,  hp), Vector3(0, 0, 1), 1.0f, 1.0f);
        VerticeModelado v3(Vector3(-ha,  hb,  hp), Vector3(0, 0, 1), 0.0f, 1.0f);
        malla.AgregarTriangulo(v0, v1, v2);
        malla.AgregarTriangulo(v0, v2, v3);

        // Cara Trasera
        VerticeModelado v4(Vector3(-ha, -hb, -hp), Vector3(0, 0, -1), 1.0f, 0.0f);
        VerticeModelado v5(Vector3( ha, -hb, -hp), Vector3(0, 0, -1), 0.0f, 0.0f);
        VerticeModelado v6(Vector3( ha,  hb, -hp), Vector3(0, 0, -1), 0.0f, 1.0f);
        VerticeModelado v7(Vector3(-ha,  hb, -hp), Vector3(0, 0, -1), 1.0f, 1.0f);
        malla.AgregarTriangulo(v5, v4, v7);
        malla.AgregarTriangulo(v5, v7, v6);

        return malla;
    }

    // 2. Extrusión de caras (Permite modelar muros, volúmenes o geometrías extruidas a partir de un plano base)
    static void ExtruirMalla(MallaEditable& malla, float distanciaExtrusion, const Vector3& direccionNormal) {
        size_t verticesActuales = malla.Vertices.size();
        std::vector<VerticeModelado> nuevosVertices;
        nuevosVertices.reserve(verticesActuales);

        // Duplicar y desplazar vértices a lo largo de la dirección de extrusión
        for (size_t i = 0; i < verticesActuales; ++i) {
            VerticeModelado v = malla.Vertices[i];
            v.Posicion.X += direccionNormal.X * distanciaExtrusion;
            v.Posicion.Y += direccionNormal.Y * distanciaExtrusion;
            v.Posicion.Z += direccionNormal.Z * distanciaExtrusion;
            nuevosVertices.push_back(v);
        }

        // Agregar los nuevos vértices y conectar las caras laterales (puentes)
        for (size_t i = 0; i < verticesActuales; ++i) {
            malla.Vertices.push_back(nuevosVertices[i]);
        }
    }

    // 3. Herramienta de subdivisión / suavizado de vértices (Deformación de escultura simple)
    static void AplicarDeformacionRadial(MallaEditable& malla, const Vector3& centroInteres, float radio, float fuerza) {
        for (auto& v : malla.Vertices) {
            float distancia = std::sqrt(
                std::pow(v.Posicion.X - centroInteres.X, 2) +
                std::pow(v.Posicion.Y - centroInteres.Y, 2) +
                std::pow(v.Posicion.Z - centroInteres.Z, 2)
            );

            if (distancia < radio) {
                // Atenuación suave basada en la distancia (caída de pincel)
                float factor = 1.0f - (distancia / radio);
                v.Posicion.Y += fuerza * factor; // Eleva o hunde los vértices en el eje Y
            }
        }
    }

    // 4. Conversión de MallaEditable a un recurso de `Malla` listo para renderizado en el motor
    static std::shared_ptr<Malla> ExportarAMallaMotor(const MallaEditable& mallaEditable) {
        auto mallaGPU = std::make_shared<Malla>();
        // Aquí se enlazarían los buffers de vértices e índices hacia el motor de render (OpenGL / Vulkan)
        // utilizando mallaEditable.Vertices y mallaEditable.Indices.
        return mallaGPU;
    }
};
