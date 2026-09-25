#pragma once
#include <iostream>
#include <cstdint>
#include "GestorEntidades.hpp"
#include "Componentes.hpp"
#include "SistemaOptica.hpp"
#include "SistemaAerodinamica.hpp"
#include "SistemaAnimacion.hpp"

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
        // INSPECCIÓN DE SUBSISTEMAS NUEVOS (ÓPTICA, AERODINÁMICA Y ANIMACIÓN)
        // ==========================================
        ComponenteOptico* optico = gestor.ObtenerComponente<ComponenteOptico>(entidad);
        if (optico) {
            std::cout << "[Componente Optico - Fermat]\n";
            std::cout << "  Indice Refraccion (n) : " << optico->IndiceRefraction << "\n";
            std::cout << "  Radio de Influencia   : " << optico->RadioInfluencia << "\n";
            std::cout << "  Activo                : " << (optico->Activo ? "SI" : "NO") << "\n";
        }

        ComponenteAerodinamico* aero = gestor.ObtenerComponente<ComponenteAerodinamico>(entidad);
        if (aero) {
            std::cout << "[Componente Aerodinamico]\n";
            std::cout << "  Coef. Arrastre (Cd)   : " << aero->CoeficienteArrastre << "\n";
            std::cout << "  Area Frontal          : " << aero->AreaFrontal << " m^2\n";
            std::cout << "  Sustentacion (Lift)   : " << aero->CoeficienteSustentacion << "\n";
            std::cout << "  Activo                : " << (aero->Activo ? "SI" : "NO") << "\n";
        }

        ComponenteAnimacion* anim = gestor.ObtenerComponente<ComponenteAnimacion>(entidad);
        if (anim) {
            std::cout << "[Componente Animacion Keyframes]\n";
            std::cout << "  Clip Actual           : " << anim->NombreClip << "\n";
            std::cout << "  Duracion Total        : " << anim->Duracion << "s\n";
            std::cout << "  Tiempo Transcurrido   : " << anim->TiempoActual << "s\n";
            std::cout << "  Reproduciendo         : " << (anim->EnReproduccion ? "SI" : "NO") << "\n";
            std::cout << "  Bucle                 : " << (anim->EnBucle ? "SI" : "NO") << "\n";
        }

        // ==========================================
        // MATERIAL Y SHADERS (PRESERVADO DE TU CÓDIGO)
        // ==========================================
        std::cout << "[Material y Shaders - Integrado]\n";
        std::cout << "  Shader Activo: PBR_Standard_Tensor\n";
        std::cout << "  Reflectividad: 0.75\n";

        std::cout << "====================================\n\n";
    }
};
