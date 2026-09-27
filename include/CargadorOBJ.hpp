#pragma once
#include "Vector3.hpp"
#include "Malla.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>

class CargadorOBJ {
public:
    static std::shared_ptr<Malla> CargarDesdeArchivo(const std::string& rutaArchivo) {
        std::vector<Vector3> posicionesTemporales;
        std::vector<Vertice> verticesFinales;
        std::vector<unsigned int> indicesFinales;

        std::ifstream archivo(rutaArchivo);
        if (!archivo.is_open()) {
            std::cerr << "[CargadorOBJ] Error crítico: No se pudo abrir el archivo OBJ en la ruta: " << rutaArchivo << "\n";
            return nullptr;
        }

        std::string linea;
        while (std::getline(archivo, linea)) {
            std::stringstream ss(linea);
            std::string prefijo;
            ss >> prefijo;

            if (prefijo == "v") {
                float x, y, z;
                ss >> x >> y >> z;
                posicionesTemporales.push_back(Vector3(x, y, z));
            } 
            else if (prefijo == "f") {
                std::string token1, token2, token3;
                ss >> token1 >> token2 >> token3;

                // Función lambda interna para extraer el índice base de un token OBJ (ej. "1/2/3" -> 1)
                auto extraerIndice = [](const std::string& token) -> unsigned int {
                    size_t posicionBarra = token.find('/');
                    std::string subIndice = (posicionBarra != std::string::npos) ? token.substr(0, posicionBarra) : token;
                    return std::stoul(subIndice);
                };

                unsigned int i1 = extraerIndice(token1);
                unsigned int i2 = extraerIndice(token2);
                unsigned int i3 = extraerIndice(token3);

                // Conversión de índices 1-based (estándar OBJ) a 0-based (C++)
                indicesFinales.push_back(i1 - 1);
                indicesFinales.push_back(i2 - 1);
                indicesFinales.push_back(i3 - 1);
            }
            // Las líneas de comentarios (#), materiales (mtllib/usemtl) o normales (vn) se ignoran de forma segura
        }

        if (posicionesTemporales.empty() || indicesFinales.empty()) {
            std::cerr << "[CargadorOBJ] Advertencia: El archivo OBJ está vacío o no contiene geometría válida: " << rutaArchivo << "\n";
            return nullptr;
        }

        // Construcción de la malla final basada en las posiciones leídas
        for (const auto& pos : posicionesTemporales) {
            verticesFinales.push_back({ pos });
        }

        auto nuevaMalla = std::make_shared<Malla>(verticesFinales, indicesFinales);
        nuevaMalla->InicializarEnGPU();

        std::cout << "[CargadorOBJ] Modelo cargado con éxito: " << rutaArchivo 
                  << " (" << verticesFinales.size() << " vértices, " << (indicesFinales.size() / 3) << " triángulos)." << std::endl;

        return nuevaMalla;
    }
};
