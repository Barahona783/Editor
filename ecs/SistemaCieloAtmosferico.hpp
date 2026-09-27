#pragma once
#include "../include/Vector3.hpp"
#include <string>
#include <cmath>
#include <algorithm>

// ==========================================
// COMPONENTE DE CIELO Y ATMÓSFERA ESTILIZADA
// ==========================================
struct ComponenteCieloAtmosferico {
    std::string Nombre;
    Vector3 ColorZenit;     // Color en la parte más alta del cielo
    Vector3 ColorHorizonte; // Color en la línea de horizonte
    Vector3 ColorSol;       // Color del disco solar o foco de luz principal
    float DensidadNubes;    // Densidad y cobertura de las capas de nubes estilizadas
    float AlturaCapaNubes;  // Altura en el Eje Y de la capa atmosférica de nubes
    bool Activo;

    ComponenteCieloAtmosferico(std::string nombre = "CieloProcedural",
                               Vector3 zenit = Vector3(0.2f, 0.4f, 0.8f),
                               Vector3 horizonte = Vector3(0.7f, 0.8f, 0.9f),
                               Vector3 sol = Vector3(1.0f, 0.95f, 0.8f),
                               float densidadNubes = 0.4f,
                               float alturaNubes = 50.0f)
        : Nombre(nombre), ColorZenit(zenit), ColorHorizonte(horizonte), 
          ColorSol(sol), DensidadNubes(densidadNubes), AlturaCapaNubes(alturaNubes), Activo(true) {}
};

// ==========================================
// SUBSISTEMA DE CIELO Y ATMÓSFERA
// ==========================================
class SistemaCieloAtmosferico {
public:

    // Calcula el color interpolado del cielo en función de la dirección de la visual
    static Vector3 CalcularColorCeleste(const Vector3& direccionVisual, const ComponenteCieloAtmosferico& cielo) {
        float factorVertical = std::max(0.0f, std::min(1.0f, direccionVisual.Y));
        
        // Interpolación lineal (lerp) entre el horizonte y el zenit
        float r = cielo.ColorHorizonte.X + (cielo.ColorZenit.X - cielo.ColorHorizonte.X) * factorVertical;
        float g = cielo.ColorHorizonte.Y + (cielo.ColorZenit.Y - cielo.ColorHorizonte.Y) * factorVertical;
        float b = cielo.ColorHorizonte.Z + (cielo.ColorZenit.Z - cielo.ColorHorizonte.Z) * factorVertical;

        return Vector3(r, g, b);
    }

    // Actualiza el estado atmosférico y las capas de nubes en función del tiempo del mundo
    template <typename GestorEntidadesT>
    static void Actualizar(GestorEntidadesT& gestorEntidades, float deltaTime) {
        const auto& entidades = gestorEntidades.ObtenerTodasLasEntidades();
        for (const auto& entidad : entidades) {
            ComponenteCieloAtmosferico* cielo = gestorEntidades.template ObtenerComponente<ComponenteCieloAtmosferico>(entidad);
            if (cielo && cielo->Activo) {
                // Procesamiento de desplazamiento dinámico de nubes o ciclo día/noche estilizado
                (void)deltaTime; 
            }
        }
    }
};
