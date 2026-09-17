#pragma once
#include "Malla.hpp"
#include "Textura.hpp"
#include <string>
#include <unordered_map>
#include <memory>
#include <iostream>

class GestorRecursos {
private:
    std::unordered_map<std::string, std::shared_ptr<Malla>> m_Mallas;
    std::unordered_map<std::string, std::shared_ptr<Textura>> m_Texturas;

public:
    static GestorRecursos& ObtenerInstancia() {
        static GestorRecursos instancia;
        return instancia;
    }

    std::shared_ptr<Malla> CargarMalla(const std::string& clave, const std::vector<Vertice>& vertices, const std::vector<unsigned int>& indices) {
        if (m_Mallas.find(clave) != m_Mallas.end()) {
            return m_Mallas[clave];
        }
        auto nuevaMalla = std::make_shared<Malla>(vertices, indices);
        nuevaMalla->InicializarEnGPU();
        m_Mallas[clave] = nuevaMalla;
        return nuevaMalla;
    }

    std::shared_ptr<Textura> CargarTexturaBMP(const std::string& ruta) {
        if (m_Texturas.find(ruta) != m_Texturas.end()) {
            return m_Texturas[ruta];
        }
        auto nuevaTextura = std::make_shared<Textura>();
        if (nuevaTextura->CargarDesdeBMP(ruta)) {
            m_Texturas[ruta] = nuevaTextura;
            return nuevaTextura;
        }
        return nullptr;
    }

    void LimpiarRecursos() {
        m_Mallas.clear();
        m_Texturas.clear();
        std::cout << "[GestorRecursos] Memoria de recursos liberada correctamente.\n";
    }
};
