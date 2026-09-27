#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <memory>
#include <any>
#include <typeindex>
#include <iostream>
#include <mutex>

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
    mutable std::mutex m_MutexBus; // Garantiza seguridad ante concurrencia multihilo en el motor

    BusEventos() = default;

public:
    // Patrón Singleton para acceso global y directo desde cualquier sistema nativo del motor
    static BusEventos& ObtenerInstancia() {
        static BusEventos instancia;
        return instancia;
    }

    // Suscribirse a un tipo de evento específico de forma segura para hilos
    template<typename T>
    void Suscribir(std::function<void(const T&)> callback) {
        std::lock_guard<std::mutex> bloqueo(m_MutexBus);
        std::type_index tipo(typeid(T));
        
        if (m_Suscriptores.find(tipo) == m_Suscriptores.end()) {
            m_Suscriptores[tipo] = std::make_unique<CallbackLista<T>>();
        }
        
        auto* lista = static_cast<CallbackLista<T>*>(m_Suscriptores[tipo].get());
        lista->callbacks.push_back(callback);
    }

    // Disparar un evento de manera inmediata a todos los suscriptores de forma segura
    template<typename T>
    void Publicar(const T& evento) {
        // Bloqueamos la obtención de la lista, pero copiamos los callbacks localmente 
        // para evitar deadlocks si un callback decide suscribir o publicar otro evento de inmediato.
        std::vector<std::function<void(const T&)>> callbacksLocales;
        {
            std::lock_guard<std::mutex> bloqueo(m_MutexBus);
            std::type_index tipo(typeid(T));
            auto it = m_Suscriptores.find(tipo);
            if (it != m_Suscriptores.end()) {
                auto* lista = static_cast<CallbackLista<T>*>(it->second.get());
                callbacksLocales = lista->callbacks;
            }
        }

        // Ejecución de los callbacks fuera de la sección crítica del mutex principal
        for (const auto& callback : callbacksLocales) {
            callback(evento);
        }
    }

    // Evitar copias (Singleton estricto)
    BusEventos(const BusEventos&) = delete;
    BusEventos& operator=(const BusEventos&) = delete;
};
