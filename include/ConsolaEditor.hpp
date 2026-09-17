#pragma once
#include <vector>
#include <string>
#include <iostream>

struct MensajeLog {
    std::string Texto;
    std::string Tipo; // "INFO", "WARN", "ERROR"
};

class ConsolaEditor {
private:
    std::vector<MensajeLog> m_Logs;

public:
    static ConsolaEditor& ObtenerInstancia() {
        static ConsolaEditor instancia;
        return instancia;
    }

    void AgregarLog(const std::string& mensaje, const std::string& tipo = "INFO") {
        m_Logs.push_back({ mensaje, tipo });
        std::cout << "[" << tipo << "] " << mensaje << "\n";
    }

    const std::vector<MensajeLog>& ObtenerLogs() const {
        return m_Logs;
    }

    void Limpiar() {
        m_Logs.clear();
    }
};
