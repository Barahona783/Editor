#pragma once
#include "GestorEntidades.hpp"

// Interfaz base abstracta pura para cualquier plugin o sistema externo del motor.
// Ningún plugin tiene acceso al código fuente del núcleo, solo interactúa a través de esta interfaz segura.
class IPlugin {
public:
    virtual ~IPlugin() = default;

    // Se ejecuta al cargar el plugin en el motor
    virtual void Inicializar(GestorEntidades& gestor) = 0;

    // Se ejecuta en cada ciclo del bucle principal del juego/editor
    virtual void Actualizar(GestorEntidades& gestor, float deltaTime) = 0;

    // Se ejecuta al descargar o cerrar el motor
    virtual void Destruir(GestorEntidades& gestor) = 0;

    // Permite obtener el nombre identificador del plugin
    virtual const char* ObtenerNombre() const = 0;
};

// Definición del tipo de función para la fábrica de creación del plugin en tiempo de ejecución (dinámico)
typedef IPlugin* (*CrearPluginFunc)();
typedef void (*DestruirPluginFunc)(IPlugin*);
