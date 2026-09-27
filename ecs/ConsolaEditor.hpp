#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <mutex>

// ==========================================
// SISTEMA DE CONSOLA DE DIAGNÓSTICO DEL EDITOR
// ==========================================
enum class TipoLog {
    Info,
    Advertencia,
    Error,
    Memoria
};

struct MensajeConsola {
    std::string Texto;
    TipoLog Tipo;
};

class ConsolaEditor {
private:
    std::vector<MensajeConsola> m_Mensajes;
    size_t m_CapacidadMaxima;
    mutable std::mutex m_MutexConsola; // Garantiza seguridad ante registros concurrentes desde múltiples hilos

public:
    ConsolaEditor(size_t capacidadMaxima = 50) : m_CapacidadMaxima(capacidadMaxima) {}

    // Registra un mensaje de forma segura permitiendo clasificar su categoría de diagnóstico
    void RegistrarMensaje(const std::string& mensaje, TipoLog tipo = TipoLog::Info) {
        std::lock_guard<std::mutex> bloqueo(m_MutexConsola);

        if (m_Mensajes.size() >= m_CapacidadMaxima) {
            m_Mensajes.erase(m_Mensajes.begin());
        }
        m_Mensajes.push_back({mensaje, tipo});

        // Salida rápida en terminal para depuración en vivo (exclusivo para builds de depuración)
        #ifdef _DEBUG
        std::string prefijo = "[INFO]";
        if (tipo == TipoLog::Advertencia) prefijo = "[ADVERTENCIA]";
        else if (tipo == TipoLog::Error) prefijo = "[ERROR]";
        else if (tipo == TipoLog::Memoria) prefijo = "[MEMORIA]";

        std::cout << "[ConsolaEditor] " << prefijo << " " << mensaje << std::endl;
        #endif
    }

    void Limpiar() {
        std::lock_guard<std::mutex> bloqueo(m_MutexConsola);
        m_Mensajes.clear();
    }

    const std::vector<MensajeConsola>& ObtenerMensajes() const {
        std::lock_guard<std::mutex> bloqueo(m_MutexConsola);
        return m_Mensajes;
    }

    // Renderizado visual del panel inferior de la consola
    void RenderizarConsola() {
        // Aquí se dibujarán las líneas de texto formateadas según su TipoLog
        // dentro del rectángulo inferior de la UI flotante del editor.
    }
};
