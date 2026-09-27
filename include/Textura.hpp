#pragma once
#include <GL/gl.h>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>

class Textura {
private:
    unsigned int m_IDTextura;
    unsigned int m_Ancho;
    unsigned int m_Alto;

public:
    Textura() : m_IDTextura(0), m_Ancho(0), m_Alto(0) {}

    ~Textura() {
        Liberar();
    }

    // Evitar copias accidentales para prevenir doble liberación de recursos en GPU
    Textura(const Textura&) = delete;
    Textura& operator=(const Textura&) = delete;

    // Permitir movimientos
    Textura(Textura&& otro) noexcept 
        : m_IDTextura(otro.m_IDTextura), m_Ancho(otro.m_Ancho), m_Alto(otro.m_Alto) {
        otro.m_IDTextura = 0;
        otro.m_Ancho = 0;
        otro.m_Alto = 0;
    }

    Textura& operator=(Textura&& otro) noexcept {
        if (this != &otro) {
            Liberar();
            m_IDTextura = otro.m_IDTextura;
            m_Ancho = otro.m_Ancho;
            m_Alto = otro.m_Alto;
            otro.m_IDTextura = 0;
            otro.m_Ancho = 0;
            otro.m_Alto = 0;
        }
        return *this;
    }

    void Liberar() {
        if (m_IDTextura != 0) {
            glDeleteTextures(1, &m_IDTextura);
            m_IDTextura = 0;
        }
    }

    bool CargarBMP(const std::string& rutaArchivo) {
        std::ifstream archivo(rutaArchivo, std::ios::binary);
        if (!archivo.is_open()) {
            std::cerr << "Error al abrir la textura BMP: " << rutaArchivo << "\n";
            return false;
        }

        unsigned char cabecera[54];
        archivo.read(reinterpret_cast<char*>(cabecera), 54);

        if (cabecera[0] != 'B' || cabecera[1] != 'M') {
            std::cerr << "El archivo no es un BMP valido: " << rutaArchivo << "\n";
            return false;
        }

        // Extracción segura de metadatos de la cabecera BMP
        unsigned int dataOffset = *reinterpret_cast<unsigned int*>(&(cabecera[10]));
        m_Ancho = *reinterpret_cast<int*>(&(cabecera[18]));
        m_Alto = *reinterpret_cast<int*>(&(cabecera[22]));
        unsigned int tamanoDatos = *reinterpret_cast<int*>(&(cabecera[34]));

        if (tamanoDatos == 0) {
            tamanoDatos = m_Ancho * m_Alto * 3;
        }

        // Posicionarse exactamente donde comienzan los datos de píxeles según la cabecera
        if (dataOffset != 0) {
            archivo.seekg(dataOffset, std::ios::beg);
        }

        std::vector<unsigned char> datos(tamanoDatos);
        archivo.read(reinterpret_cast<char*>(datos.data()), tamanoDatos);

        // Generación y configuración del objeto de textura en la GPU
        glGenTextures(1, &m_IDTextura);
        glBindTexture(GL_TEXTURE_2D, m_IDTextura);

        // Configuración de filtrado y muestreo lineal por defecto
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Ancho, m_Alto, 0, GL_BGR, GL_UNSIGNED_BYTE, datos.data());
        glBindTexture(GL_TEXTURE_2D, 0);

        return true;
    }

    void Activar(unsigned int unidadSlot = 0) const {
        glActiveTexture(GL_TEXTURE0 + unidadSlot);
        glBindTexture(GL_TEXTURE_2D, m_IDTextura);
    }

    void Desactivar() const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    unsigned int ObtenerID() const { return m_IDTextura; }
    unsigned int ObtenerAncho() const { return m_Ancho; }
    unsigned int ObtenerAlto() const { return m_Alto; }
};
