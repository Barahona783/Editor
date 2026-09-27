#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <dlfcn.h>
#include "IPlugin.hpp"
#include "GestorEntidades.hpp"

// ==========================================
// GESTOR DE PLUGINS DINÁMICOS Y MÓDULOS EXTERNOS
// ==========================================
class GestorPlugins {
private:
    struct PluginInstancia {
        void* handleBiblioteca;
        IPlugin* punteroPlugin;
        std::string rutaArchivo;
    };

    std::vector<PluginInstancia> m_Plugins;

public:
    GestorPlugins() = default;

    ~GestorPlugins() {
        DescargarTodosLosPlugins();
    }

    // Carga un plugin dinámico (.so) en tiempo de ejecución de forma segura
    bool CargarPlugin(const std::string& rutaSo, GestorEntidades& gestor) {
        // Abrir la librería compartida con resolución inmediata de símbolos
        void* handle = dlopen(rutaSo.c_str(), RTLD_NOW);
        if (!handle) {
            #ifdef _DEBUG
            std::cerr << "[GestorPlugins] Error al cargar el plugin [" << rutaSo << "]: " << dlerror() << "\n";
            #endif
            return false;
        }

        // Limpiar errores previos de dlsym
        dlerror();

        // Buscar la función de fábrica para la creación de la instancia del plugin
        CrearPluginFunc crearPlugin = (CrearPluginFunc)dlsym(handle, "CrearPlugin");
        const char* dlsymError = dlerror();
        if (dlsymError || !crearPlugin) {
            #ifdef _DEBUG
            std::cerr << "[GestorPlugins] Error al buscar el símbolo 'CrearPlugin' en " << rutaSo << ": " << (dlsymError ? dlsymError : "Desconocido") << "\n";
            #endif
            dlclose(handle);
            return false;
        }

        // Instanciar el plugin a través de la interfaz abstracta
        IPlugin* plugin = crearPlugin();
        if (!plugin) {
            #ifdef _DEBUG
            std::cerr << "[GestorPlugins] La fábrica 'CrearPlugin' devolvió un puntero nulo en: " << rutaSo << "\n";
            #endif
            dlclose(handle);
            return false;
        }

        // Inicializar el plugin pasándole el gestor autorizado del motor
        plugin->Inicializar(gestor);

        m_Plugins.push_back({handle, plugin, rutaSo});
        
        #ifdef _DEBUG
        std::cout << "[GestorPlugins] Plugin cargado con éxito: " << plugin->ObtenerNombre() << " (" << rutaSo << ")\n";
        #endif
        return true;
    }

    // Actualiza todos los plugins activos en cada frame del bucle principal del motor
    void ActualizarPlugins(GestorEntidades& gestor, float deltaTime) {
        for (auto& instancia : m_Plugins) {
            if (instancia.punteroPlugin) {
                instancia.punteroPlugin->Actualizar(gestor, deltaTime);
            }
        }
    }

    // Descarga y libera todos los plugins de manera limpia y ordenada
    void DescargarTodosLosPlugins() {
        for (auto& instancia : m_Plugins) {
            if (instancia.punteroPlugin) {
                delete instancia.punteroPlugin;
                instancia.punteroPlugin = nullptr;
            }
            if (instancia.handleBiblioteca) {
                dlclose(instancia.handleBiblioteca);
                instancia.handleBiblioteca = nullptr;
            }
        }
        m_Plugins.clear();
        
        #ifdef _DEBUG
        std::cout << "[GestorPlugins] Todos los plugins externos han sido descargados de forma segura.\n";
        #endif
    }
};
