#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <memory>
#include <any>
#include <typeindex>
#include <iostream>

class BusEventos {
private:
    // Estructura interna para almacenar callbacks genéricos por tipo de evento
    struct ICallbackLista {
        virtual ~ICallbackLista() = default;
    };

    template<typename T>
    struct CallbackLista : public ICallbackLista {
        std::vector<std::function<void(const T&)>> callbacks;
    };

    std::unordered_map<std::type_index, std::unique_ptr<ICallbackLista>> m_Suscriptores;

    BusEventos() = default;

public:
    // Patrón Singleton para acceso global y directo desde cualquier sistema nativo del motor
    static BusEventos& ObtenerInstancia() {
        static BusEventos instancia;
        return instancia;
    }

    // Suscribirse a un tipo de evento específico
    template<typename T>
    void Suscribir(std::function<void(const T&)> callback) {
        std::type_index tipo(typeid(T));
        if (m_Suscriptores.find(tipo) == m_Suscriptores.end()) {
            m_Suscriptores[tipo] = std::make_unique<CallbackLista<T>>();
        }
        
        auto* lista = static_cast<CallbackLista<T>*>(m_Suscriptores[tipo].get());
        lista->callbacks.push_back(callback);
    }

    // Disparar un evento de manera inmediata a todos los suscriptores
    template<typename T>
    void Publicar(const T& evento) {
        std::type_index tipo(typeid(T));
        auto it = m_Suscriptores.find(tipo);
        if (it != m_Suscriptores.end()) {
            auto* lista = static_cast<CallbackLista<T>*>(it->second.get());
            for (auto& callback : lista->callbacks) {
                callback(evento);
            }
        }
    }

    // Evitar copias (Singleton estricto)
    BusEventos(const BusEventos&) = delete;
    BusEventos& operator=(const BusEventos&) = delete;
};
