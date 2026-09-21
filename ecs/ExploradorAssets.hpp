#pragma once
#include <vector>
#include <string>
#include <iostream>

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
        m_Assets.push_back({ "CuboModelo.obj", "Modelo3D" });
        m_Assets.push_back({ "TexturaPared.png", "Textura" });
        m_Assets.push_back({ "ScriptIA.cpp", "Script" });
    }

    void AlternarExplorador() { m_MostrarExplorador = !m_MostrarExplorador; }
    bool EstaActivo() const { return m_MostrarExplorador; }

    const std::vector<AssetArchivo>& ObtenerAssets() const {
        return m_Assets;
    }

    void RegistrarAsset(const std::string& nombre, const std::string& tipo) {
        m_Assets.push_back({ nombre, tipo });
        std::cout << "[ExploradorAssets] Asset añadido: " << nombre << " (" << tipo << ")\n";
    }
};
