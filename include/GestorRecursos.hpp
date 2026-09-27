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

    // Constructor privado para garantizar el patrón Singleton estricto
    GestorRecursos() = default;
    
    ~GestorRecursos() {
        LimpiarRecursos();
    }

public:
    // Eliminar constructor de copia, movimiento y operadores de asignación por seguridad estricta
    GestorRecursos(const GestorRecursos&) = delete;
    GestorRecursos& operator=(const GestorRecursos&) = delete;
    GestorRecursos(GestorRecursos&&) = delete;
    GestorRecursos& operator=(GestorRecursos&&) = delete;

    // Instancia global única del motor (Meyers' Singleton)
    static GestorRecursos& ObtenerInstancia() {
        static GestorRecursos instancia;
        return instancia;
    }

    // Carga o recupera una malla de la memoria caché del motor
    std::shared_ptr<Malla> CargarMalla(const std::string& clave, const std::vector<Vertice>& vertices, const std::vector<unsigned int>& indices) {
        auto it = m_Mallas.find(clave);
        if (it != m_Mallas.end()) {
            return it->second;
        }

        auto nuevaMalla = std::make_shared<Malla>(vertices, indices);
        if (nuevaMalla) {
            nuevaMalla->InicializarEnGPU();
            m_Mallas[clave] = nuevaMalla;
        }
        return nuevaMalla;
    }

    // Carga o recupera una textura BMP desde la caché del motor
    std::shared_ptr<Textura> CargarTexturaBMP(const std::string& ruta) {
        auto it = m_Texturas.find(ruta);
        if (it != m_Texturas.end()) {
            return it->second;
        }

        auto nuevaTextura = std::make_shared<Textura>();
        if (nuevaTextura && nuevaTextura->CargarDesdeBMP(ruta)) {
            m_Texturas[ruta] = nuevaTextura;
            return nuevaTextura;
        }

        std::cerr << "[GestorRecursos] Advertencia: No se pudo cargar la textura BMP: " << ruta << "\n";
        return nullptr;
    }

    // Libera todos los recursos al cerrar o reiniciar el motor
    void LimpiarRecursos() {
        m_Mallas.clear();
        m_Texturas.clear();
        std::cout << "[GestorRecursos] Memoria de recursos y caché liberada correctamente.\n";
    }
};
