#pragma once
#include <string>
#include <vector>

// ==========================================
// SUBSISTEMA DE PLANOS ARQUITECTÓNICOS Y CAD (BIM)
// Gestión de Muros, Capas, Materiales y Propiedades Estructurales
// ==========================================

struct ComponentePlanoArquitectonico {
    std::string NombreUnidad;     // Identificador de la unidad (ej. "Unit 101 - Lvl 2")
    float EspesorMuroMM;          // Espesor del muro en milímetros (ej. 120.0f)
    float LongitudMuroMM;         // Longitud del segmento (ej. 5500.0f)
    float AlturaMuroMM;           // Altura del piso al techo (ej. 2900.0f)
    
    // Control de Capas (Layers Palette)
    bool CapaMurosVisibles;
    bool CapaMueblesVisibles;
    bool CapaDimensionesVisibles;
    float PorcentajeMuros;        // Intensidad o uso de capa (ej. 30%)
    float PorcentajeMuebles;      // (ej. 50%)

    // Selector de Materiales (Material Selector)
    std::string MaterialSeleccionado; // "Ash Wood Flooring", "Concrete Wall", "Copper Cladding", "Marble"

    ComponentePlanoArquitectonico(const std::string& unidad = "Unit 101 - Lvl 2")
        : NombreUnidad(unidad), EspesorMuroMM(120.0f), LongitudMuroMM(5500.0f), AlturaMuroMM(2900.0f),
          CapaMurosVisibles(true), CapaMueblesVisibles(true), CapaDimensionesVisibles(true),
          PorcentajeMuros(30.0f), PorcentajeMuebles(50.0f),
          MaterialSeleccionado("Concrete Wall") {}
};

class SistemaPlanoArquitectonico {
public:
    // Actualiza las propiedades físicas y dimensionales del muro seleccionado
    static void ActualizarPropiedadesMuro(ComponentePlanoArquitectonico& plano, float espesor, float longitud, float altura) {
        plano.EspesorMuroMM = espesor > 0.0f ? espesor : 120.0f;
        plano.LongitudMuroMM = longitud > 0.0f ? longitud : 100.0f;
        plano.AlturaMuroMM = altura > 0.0f ? altura : 2900.0f;
    }

    // Cambia el material activo de renderizado para los elementos arquitectónicos
    static void AsignarMaterial(ComponentePlanoArquitectonico& plano, const std::string& material) {
        plano.MaterialSeleccionado = material;
    }

    // Ciclo de actualización del sistema de planos en el motor
    template <typename GestorEntidadesTipo>
    static void Actualizar(GestorEntidadesTipo& gestorEntidades) {
        auto entidades = gestorEntidades.ObtenerTodasLasEntidades();
        for (auto entidad : entidades) {
            ComponentePlanoArquitectonico* plano = gestorEntidades.template ObtenerComponente<ComponentePlanoArquitectonico>(entidad);
            if (plano) {
                // Aquí se puede procesar la sincronización entre el plano 2D y la malla 3D generada por SistemaModelado3D
            }
        }
    }
};
