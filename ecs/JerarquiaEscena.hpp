#pragma once
#include "GestorEntidades.hpp"
#include <vector>
#include <string>
#include <iostream>

class JerarquiaEscena {
private:
    Entidad m_EntidadSeleccionada;

public:
    JerarquiaEscena() : m_EntidadSeleccionada(0) {}

    void SeleccionarEntidad(Entidad entidad) {
        m_EntidadSeleccionada = entidad;
    }

    Entidad ObtenerEntidadSeleccionada() const {
        return m_EntidadSeleccionada;
    }

    void RenderizarArbol(GestorEntidades& gestor) {
        const auto& entidades = gestor.ObtenerTodasLasEntidades();
        std::cout << "--- JERARQUIA DE LA ESCENA ---\n";
        for (const auto& e : entidades) {
            auto nom = gestor.ObtenerNombre(e);
            std::string etiqueta = nom ? nom->Nombre : "Entidad_" + std::to_string(e.ObtenerID());
            
            if (e.ObtenerID() == m_EntidadSeleccionada.ObtenerID()) {
                std::cout << " > [" << e.ObtenerID() << "] " << etiqueta << " (SELECCIONADO)\n";
            } else {
                std::cout << "   [" << e.ObtenerID() << "] " << etiqueta << "\n";
            }
        }
    }
};
