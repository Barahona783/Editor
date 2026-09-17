#pragma once
#include <vector>
#include <string>
#include <iostream>

class ConsolaEditor {
private:
    std::vector<std::string> m_Mensajes;
    size_t m_CapacidadMaxima;

public:
    ConsolaEditor(size_t capacidadMaxima = 50) : m_CapacidadMaxima(capacidadMaxima) {}

    void RegistrarMensaje(const std::string& mensaje) {
        if (m_Mensajes.size() >= m_CapacidadMaxima) {
            m_Mensajes.erase(m_Mensajes.begin());
        }
        m_Mensajes.push_back(mensaje);
    }

    void Limpiar() {
        m_Mensajes.clear();
    }

    const std::vector<std::string>& ObtenerMensajes() const {
        return m_Mensajes;
    }

    // Renderizado visual del panel inferior de la consola
    void RenderizarConsola() {
        // Aquí se dibujarán las líneas de texto dentro del rectángulo inferior de la UI
    }
};
