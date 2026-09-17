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
        if (m_IDTextura != 0) {
            glDeleteTextures(1, &m_IDTextura);
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

        m_Ancho = *reinterpret_cast<int*>(&(cabecera[18]));
        m_Alto = *reinterpret_cast<int*>(&(cabecera[22]));
        unsigned int tamanoDatos = *reinterpret_cast<int*>(&(cabecera[34]));
        if (tamanoDatos == 0) tamanoDatos = m_Ancho * m_Alto * 3;

        std::vector<unsigned char> datos(tamanoDatos);
        archivo.read(reinterpret_cast<char*>(datos.data()), tamanoDatos);

        glGenTextures(1, &m_IDTextura);
        glBindTexture(GL_TEXTURE_2D, m_IDTextura);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Ancho, m_Alto, 0, GL_BGR, GL_UNSIGNED_BYTE, datos.data());
        glBindTexture(GL_TEXTURE_2D, 0);

        return true;
    }

    void Activar() const {
        glBindTexture(GL_TEXTURE_2D, m_IDTextura);
    }

    void Desactivar() const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};
