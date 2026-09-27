#pragma once
#include "GestorEntidades.hpp"

// ==========================================
// INTERFAZ BASE PARA PLUGINS Y MÓDULOS EXTERNOS
// ==========================================
class IPlugin {
public:
    virtual ~IPlugin() = default;

    // Se ejecuta al cargar y registrar el plugin en el motor
    virtual void Inicializar(GestorEntidades& gestor) = 0;

    // Se ejecuta en cada ciclo del bucle principal del juego o del editor
    virtual void Actualizar(GestorEntidades& gestor, float deltaTime) = 0;

    // Se ejecuta al descargar el plugin o cerrar el motor para liberar recursos
    virtual void Destruir(GestorEntidades& gestor) = 0;

    // Permite obtener el nombre identificador único del plugin
    virtual const char* ObtenerNombre() const = 0;
};

// ==========================================
// DEFINICIÓN DE FÁBRICAS PARA CARGA DINÁMICA
// ==========================================
typedef IPlugin* (*CrearPluginFunc)();
typedef void (*DestruirPluginFunc)(IPlugin*);
