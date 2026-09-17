#pragma once
#include <GL/gl.h>
#include <iostream>
#include <string>

class Sombreador {
private:
    unsigned int m_IDPrograma;

    unsigned int CompilarSombreador(unsigned int tipo, const std::string& codigoFuente) {
        unsigned int id = glCreateShader(tipo);
        const char* fuente = codigoFuente.c_str();
        glShaderSource(id, 1, &fuente, nullptr);
        glCompileShader(id);

        int resultado;
        glGetShaderiv(id, GL_COMPILE_STATUS, &resultado);
        if (resultado == GL_FALSE) {
            int longitud;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &longitud);
            char* mensaje = (char*)alloca(longitud * sizeof(char));
            glGetShaderInfoLog(id, longitud, &longitud, mensaje);
            std::cerr << "Error al compilar el sombreador: " << mensaje << "\n";
            glDeleteShader(id);
            return 0;
        }
        return id;
    }

public:
    Sombreador(const std::string& codigoVertice, const std::string& codigoFragmento) {
        m_IDPrograma = glCreateProgram();
        unsigned int sv = CompilarSombreador(GL_VERTEX_SHADER, codigoVertice);
        unsigned int sf = CompilarSombreador(GL_FRAGMENT_SHADER, codigoFragmento);

        glAttachShader(m_IDPrograma, sv);
        glAttachShader(m_IDPrograma, sf);
        glLinkProgram(m_IDPrograma);
        glValidateProgram(m_IDPrograma);

        glDeleteShader(sv);
        glDeleteShader(sf);
    }

    ~Sombreador() {
        glDeleteProgram(m_IDPrograma);
    }

    void Activar() const {
        glUseProgram(m_IDPrograma);
    }

    void Desactivar() const {
        glUseProgram(0);
    }

    unsigned int ObtenerID() const { return m_IDPrograma; }
};
