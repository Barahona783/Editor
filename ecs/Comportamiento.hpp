#pragma once
#include "Entidad.hpp"
#include "GestorEntidades.hpp"
#include "Componentes.hpp"
#include <iostream>

// ==========================================
// INTERFAZ BASE DE SCRIPTS PARA ENTIDADES (ECS)
// ==========================================
class ScriptEntidad {
public:
    virtual ~ScriptEntidad() = default;

    // Se ejecuta una vez cuando la entidad nace o se inicializa en el mundo
    virtual void Iniciar(Entidad entidad, GestorEntidades& gestor) {}

    // Se ejecuta en cada frame (ideal para movimiento, lógica de juego, input o IA local)
    virtual void Actualizar(Entidad entidad, GestorEntidades& gestor, float dt) {}

    // Se ejecuta al limpiar o destruir la entidad del mundo
    virtual void Destruir(Entidad entidad, GestorEntidades& gestor) {}
};

// ==========================================
// SISTEMA DE GESTIÓN Y ACTUALIZACIÓN DE SCRIPTS
// ==========================================
class SistemaScripts {
public:
    // Actualiza de manera secuencial y segura todos los scripts activos en el mundo abierto
    static void Actualizar(GestorEntidades& gestor, float dt) {
        const auto& entidades = gestor.ObtenerTodasLasEntidades();
        
        for (auto entidad : entidades) {
            // Verificación y ejecución segura del componente de script asociado a la entidad
            auto* scriptComp = gestor.ObtenerComponente<ComponenteScript>(entidad);
            if (scriptComp && scriptComp->Instancia) {
                scriptComp->Instancia->Actualizar(entidad, gestor, dt);
            }
        }
    }
};
