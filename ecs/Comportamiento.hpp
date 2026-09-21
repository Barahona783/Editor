#pragma once
#include "Entidad.hpp"
#include "GestorEntidades.hpp"
#include <iostream>

// Clase base virtual de la cual heredarán todos los scripts de tus entidades
class ScriptEntidad {
public:
    virtual ~ScriptEntidad() = default;

    // Se ejecuta una vez cuando la entidad nace o se inicializa
    virtual void Iniciar(Entidad entidad, GestorEntidades& gestor) {}

    // Se ejecuta cada frame (ideal para movimiento, lógica de juego, input)
    virtual void Actualizar(Entidad entidad, GestorEntidades& gestor, float dt) {}

    // Se destruye al limpiar la entidad
    virtual void Destruir(Entidad entidad, GestorEntidades& gestor) {}
};

// Componente ECS que encapsula cualquier script personalizado
struct ComponenteScript {
    ScriptEntidad* Instancia = nullptr;

    // Template para asignar dinámicamente cualquier clase de script a una entidad
    template <typename T>
    void Asignar() {
        if (Instancia) delete Instancia;
        Instancia = new T();
    }

    ~ComponenteScript() {
        if (Instancia) {
            delete Instancia;
            Instancia = nullptr;
        }
    }
};

// Sistema encargado de actualizar todos los scripts activos en el mundo en cada frame
class SistemaScripts {
public:
    static void Actualizar(GestorEntidades& gestor, float dt) {
        auto entidades = gestor.ObtenerTodasLasEntidades();
        for (auto entidad : entidades) {
            // Aquí puedes iterar y llamar a Actualizar si la entidad posee el ComponenteScript
        }
    }
};
