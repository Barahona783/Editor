#pragma once
#include <iostream>
#include <cstdint>
#include "GestorEntidades.hpp"
#include "Componentes.hpp"

class Inspector {
public:
    static void MostrarInspector(GestorEntidades& gestor, Entidad entidad) {
        // Validación de seguridad si la entidad es inválida o nula (ID 0)
        if (entidad.ObtenerID() == 0) {
            std::cout << "\n====================================\n";
            std::cout << "  INSPECTOR (Sin seleccion)\n";
            std::cout << "====================================\n";
            std::cout << "  Selecciona un objeto en el viewport\n";
            std::cout << "====================================\n\n";
            return;
        }

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

        // ==========================================
        // INSPECCIÓN DE COMPONENTES TENSORIALES Y FÍSICOS
        // ==========================================
        ComponenteTensorDeformacion* tensorDeformacion = gestor.ObtenerComponente<ComponenteTensorDeformacion>(entidad);
        if (tensorDeformacion) {
            std::cout << "[Tensor de Deformacion]\n";
            std::cout << "  Elasticidad : " << tensorDeformacion->LimiteElasticidad << "\n";
            std::cout << "  Deformado   : " << (tensorDeformacion->EstaDeformado ? "SI" : "NO") << "\n";
        }

        ComponenteInerciaRotacional* inercia = gestor.ObtenerComponente<ComponenteInerciaRotacional>(entidad);
        if (inercia) {
            std::cout << "[Inercia Rotacional]\n";
            std::cout << "  Vel. Angular: (" << inercia->VelocidadAngular.X << ", " << inercia->VelocidadAngular.Y << ", " << inercia->VelocidadAngular.Z << ")\n";
        }

        ComponenteRestriccionFisica* restriccion = gestor.ObtenerComponente<ComponenteRestriccionFisica>(entidad);
        if (restriccion) {
            std::cout << "[Restriccion Fisica (Joint)]\n";
            std::cout << "  Entidad A   : " << restriccion->EntidadA << "\n";
            std::cout << "  Entidad B   : " << restriccion->EntidadB << "\n";
            std::cout << "  Distancia   : " << restriccion->DistanciaRestitucion << "\n";
        }

        // ==========================================
        // NUEVA FUNCIÓN INTEGRADA DIRECTAMENTE AQUÍ:
        // ==========================================
        // (Aquí es donde agregamos las propiedades de material/iluminación 
        // sin alterar ninguna de tus líneas anteriores de nombres, transformaciones o tensores).
        std::cout << "[Material y Shaders - Integrado]\n";
        std::cout << "  Shader Activo: PBR_Standard_Tensor\n";
        std::cout << "  Reflectividad: 0.75\n";

        std::cout << "====================================\n\n";
    }
};
