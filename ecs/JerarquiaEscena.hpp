#pragma once
#include "GestorEntidades.hpp"
#include <vector>
#include <string>
#include <iostream>

// ==========================================
// GESTOR DE JERARQUÍA Y ÁRBOL DE ESCENA
// ==========================================
class JerarquiaEscena {
private:
    Entidad m_EntidadSeleccionada;

public:
    JerarquiaEscena() : m_EntidadSeleccionada(0) {}

    // Selecciona una entidad específica en la jerarquía del mundo
    void SeleccionarEntidad(Entidad entidad) {
        m_EntidadSeleccionada = entidad;
    }

    // Devuelve la entidad actualmente seleccionada en el editor
    Entidad ObtenerEntidadSeleccionada() const {
        return m_EntidadSeleccionada;
    }

    // Renderiza el árbol completo de la escena por consola (preparado para integración futura con UI gráfica)
    void RenderizarArbol(GestorEntidades& gestor) {
        const auto& entidades = gestor.ObtenerTodasLasEntidades();
        
        std::cout << "\n====================================\n";
        std::cout << "  JERARQUIA DE LA ESCENA\n";
        std::cout << "====================================\n";
        
        if (entidades.empty()) {
            std::cout << "  (Escena vacia)\n";
        }

        for (const auto& e : entidades) {
            auto nom = gestor.ObtenerNombre(e);
            std::string etiqueta = nom ? nom->Nombre : "Entidad_" + std::to_string(e.ObtenerID());
            
            if (e.ObtenerID() == m_EntidadSeleccionada.ObtenerID()) {
                std::cout << " > [" << e.ObtenerID() << "] " << etiqueta << " (SELECCIONADO)\n";
            } else {
                std::cout << "   [" << e.ObtenerID() << "] " << etiqueta << "\n";
            }
        }
        std::cout << "====================================\n\n";
    }
};
