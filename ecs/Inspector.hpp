#pragma once
#include <iostream>
#include <cstdint>
#include "GestorEntidades.hpp"

class Inspector {
public:
    static void MostrarInspector(GestorEntidades& gestor, Entidad entidad) {
        uint32_t idEntidad = entidad.ObtenerID();
        std::cout << "\n====================================\n";
        std::cout << "  INSPECTOR DE ENTIDAD [" << idEntidad << "]\n";
        std::cout << "====================================\n";

        ComponenteNombre* nombre = gestor.ObtenerNombre(entidad);
        if (nombre) {
            std::cout << "[Nombre]\n";
            std::cout << "  Etiqueta : " << nombre->Nombre << "\n";
        }

        ComponenteTransformacion* transform = gestor.ObtenerTransformacion(entidad);
        if (transform) {
            std::cout << "[Transformacion]\n";
            std::cout << "  Posicion : (" << transform->Posicion.X << ", " << transform->Posicion.Y << ", " << transform->Posicion.Z << ")\n";
            std::cout << "  Escala   : (" << transform->Escala.X << ", " << transform->Escala.Y << ", " << transform->Escala.Z << ")\n";
        }

        ComponenteColisionador* colision = gestor.ObtenerCajaColision(entidad);
        if (colision) {
            std::cout << "[Caja de Colision]\n";
            std::cout << "  Min: (" << colision->Caja.Minimo.X << ", " << colision->Caja.Minimo.Y << ", " << colision->Caja.Minimo.Z << ")\n";
            std::cout << "  Max: (" << colision->Caja.Maximo.X << ", " << colision->Caja.Maximo.Y << ", " << colision->Caja.Maximo.Z << ")\n";
        }

        std::cout << "====================================\n\n";
    }
};
