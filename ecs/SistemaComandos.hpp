#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <sstream>
#include <iostream>
#include "GestorEntidades.hpp"
#include "BusEventos.hpp"

class SistemaComandos {
public:
    using ComandoCallback = std::function<void(const std::vector<std::string>& argumentos, GestorEntidades& gestor)>;

private:
    std::unordered_map<std::string, ComandoCallback> m_Comandos;

    SistemaComandos() {
        // Registrar comandos nativos base del motor
        RegistrarComando("ayuda", [](const auto&, auto&) {
            std::cout << "[Consola Motor] Comandos disponibles: ayuda, spawn, limpiar\n";
        });

        RegistrarComando("spawn", [](const std::vector<std::string>& args, GestorEntidades& gestor) {
            std::string nombre = args.size() > 0 ? args[0] : "EntidadDinamica";
            Entidad nueva = gestor.CrearEntidad(nombre);
            gestor.AsignarTransformacion(nueva, ComponenteTransformacion(Vector3(0.0f, 0.0f, -5.0f)));
            std::cout << "[Consola Motor] Entidad '" << nombre << "' creada con exito.\n";
        });
    }

public:
    // Patrón Singleton para acceso global inmediato desde la interfaz gráfica o consola
    static SistemaComandos& ObtenerInstancia() {
        static SistemaComandos instancia;
        return instancia;
    }

    // Permite registrar nuevos comandos personalizados en caliente
    void RegistrarComando(const std::string& nombre, ComandoCallback callback) {
        m_Comandos[nombre] = callback;
    }

    // Procesa una línea de texto introducida en la consola del editor
    bool EjecutarLinea(const std::string& lineaDeComando, GestorEntidades& gestor) {
        std::stringstream ss(lineaDeComando);
        std::string comando;
        ss >> comando;

        if (comando.empty()) return false;

        std::vector<std::string> argumentos;
        std::string arg;
        while (ss >> arg) {
            argumentos.push_back(arg);
        }

        auto it = m_Comandos.find(comando);
        if (it != m_Comandos.end()) {
            it->second(argumentos, gestor);
            return true;
        } else {
            std::cerr << "[Consola Motor] Comando desconocido: '" << comando << "'. Escribe 'ayuda' para ver la lista.\n";
            return false;
        }
    }

    SistemaComandos(const SistemaComandos&) = delete;
    SistemaComandos& operator=(const SistemaComandos&) = delete;
};
