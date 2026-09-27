#pragma once
#include <vector>
#include <string>
#include <iostream>

// ==========================================
// EXPLORADOR DE ASSETS Y RECURSOS DEL MOTOR
// ==========================================
struct AssetArchivo {
    std::string Nombre;
    std::string Tipo; // "Textura", "Modelo3D", "Script", "Shader"
};

class ExploradorAssets {
private:
    std::vector<AssetArchivo> m_Assets;
    bool m_MostrarExplorador = true;

public:
    ExploradorAssets() {
        // Assets por defecto precargados para el funcionamiento inicial del editor
        m_Assets.push_back({ "CuboModelo.obj", "Modelo3D" });
        m_Assets.push_back({ "TexturaPared.png", "Textura" });
        m_Assets.push_back({ "ScriptIA.cpp", "Script" });
    }

    // Control de visibilidad del panel en la interfaz del editor
    void AlternarExplorador() { m_MostrarExplorador = !m_MostrarExplorador; }
    bool EstaActivo() const { return m_MostrarExplorador; }

    // Consulta de la lista completa de recursos disponibles
    const std::vector<AssetArchivo>& ObtenerAssets() const {
        return m_Assets;
    }

    // Registro dinámico de nuevos assets detectados en el sistema de archivos del proyecto
    void RegistrarAsset(const std::string& nombre, const std::string& tipo) {
        // Evitar duplicados exactos en el catálogo
        for (const auto& asset : m_Assets) {
            if (asset.Nombre == nombre && asset.Tipo == tipo) {
                return;
            }
        }

        m_Assets.push_back({ nombre, tipo });
        
        #ifdef _DEBUG
        std::cout << "[ExploradorAssets] Asset añadido: " << nombre << " (" << tipo << ")\n";
        #endif
    }

    // Método para eliminar o desvincular un asset del catálogo del editor
    void RemoverAsset(const std::string& nombre) {
        for (auto it = m_Assets.begin(); it != m_Assets.end(); ++it) {
            if (it->Nombre == nombre) {
                #ifdef _DEBUG
                std::cout << "[ExploradorAssets] Asset removido: " << it->Nombre << "\n";
                #endif
                m_Assets.erase(it);
                break;
            }
        }
    }
};
