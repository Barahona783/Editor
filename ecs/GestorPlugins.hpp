#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <dlfcn.h>
#include "IPlugin.hpp"
#include "GestorEntidades.hpp"

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
        // Abrir la librería compartida
        void* handle = dlopen(rutaSo.c_str(), RTLD_NOW);
        if (!handle) {
            std::cerr << "Error al cargar el plugin [" << rutaSo << "]: " << dlerror() << "\n";
            return false;
        }

        // Limpiar errores previos de dlsym
        dlerror();

        // Buscar la función de creación de la instancia del plugin
        CrearPluginFunc crearPlugin = (CrearPluginFunc)dlsym(handle, "CrearPlugin");
        const char* dlsymError = dlerror();
        if (dlsymError || !crearPlugin) {
            std::cerr << "Error al buscar el símbolo 'CrearPlugin' en " << rutaSo << ": " << (dlsymError ? dlsymError : "Desconocido") << "\n";
            dlclose(handle);
            return false;
        }

        // Instanciar el plugin a través de la interfaz abstracta
        IPlugin* plugin = crearPlugin();
        if (!plugin) {
            std::cerr << "La fábrica 'CrearPlugin' devolvió un puntero nulo en: " << rutaSo << "\n";
            dlclose(handle);
            return false;
        }

        // Inicializar el plugin pasándole únicamente el gestor autorizado
        plugin->Inicializar(gestor);

        m_Plugins.push_back({handle, plugin, rutaSo});
        std::cout << "Plugin cargado con éxito: " << plugin->ObtenerNombre() << " (" << rutaSo << ")\n";
        return true;
    }

    // Actualiza todos los plugins activos en el bucle principal del motor
    void ActualizarPlugins(GestorEntidades& gestor, float deltaTime) {
        for (auto& instancia : m_Plugins) {
            if (instancia.punteroPlugin) {
                instancia.punteroPlugin->Actualizar(gestor, deltaTime);
            }
        }
    }

    // Descarga y libera un plugin de manera limpia
    void DescargarTodosLosPlugins() {
        // Nota: Idealmente requeriría una instancia de gestor si el destructor la exige, 
        // por lo que aseguramos limpieza segura de memoria y handles.
        for (auto& instancia : m_Plugins) {
            if (instancia.punteroPlugin) {
                delete instancia.punteroPlugin;
            }
            if (instancia.handleBiblioteca) {
                dlclose(instancia.handleBiblioteca);
            }
        }
        m_Plugins.clear();
        std::cout << "Todos los plugins externos han sido descargados de forma segura.\n";
    }
};
