#pragma once
#include "Vector3.hpp"
#include "Malla.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class CargadorOBJ {
public:
    static std::shared_ptr<Malla> CargarDesdeArchivo(const std::string& rutaArchivo) {
        std::vector<Vector3> posicionesTemporales;
        std::vector<Vertice> verticesFinales;
        std::vector<unsigned int> indicesFinales;

        std::ifstream archivo(rutaArchivo);
        if (!archivo.is_open()) {
            std::cerr << "Error al abrir el archivo OBJ: " << rutaArchivo << "\n";
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
            } else if (prefijo == "f") {
                unsigned int i1, i2, i3;
                ss >> i1 >> i2 >> i3;
                indicesFinales.push_back(i1 - 1);
                indicesFinales.push_back(i2 - 1);
                indicesFinales.push_back(i3 - 1);
            }
        }

        for (const auto& pos : posicionesTemporales) {
            verticesFinales.push_back({ pos });
        }

        auto nuevaMalla = std::make_shared<Malla>(verticesFinales, indicesFinales);
        nuevaMalla->InicializarEnGPU();
        return nuevaMalla;
    }
};
