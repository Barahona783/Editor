#pragma once
#include <GL/gl.h>
#include <iostream>
#include <string>
#include <vector>
#include "Vector3.hpp" // Asegura la compatibilidad con tipos de vectores del motor

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
            int longitud = 0;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &longitud);
            
            // Reemplazo seguro de alloca por std::vector estándar de C++
            std::vector<char> mensaje(longitud);
            glGetShaderInfoLog(id, longitud, &longitud, mensaje.data());
            
            std::cerr << "Error al compilar el sombreador (" 
                      << (tipo == GL_VERTEX_SHADER ? "Vertex" : "Fragment") 
                      << "): " << mensaje.data() << "\n";
            
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

        // Validación robusta del estado de enlazado del programa
        int estadoLink;
        glGetProgramiv(m_IDPrograma, GL_LINK_STATUS, &estadoLink);
        if (estadoLink == GL_FALSE) {
            int longitud = 0;
            glGetProgramiv(m_IDPrograma, GL_INFO_LOG_LENGTH, &longitud);
            std::vector<char> mensaje(longitud);
            glGetProgramInfoLog(m_IDPrograma, longitud, &longitud, mensaje.data());
            std::cerr << "Error al enlazar el programa de sombreador: " << mensaje.data() << "\n";
        }

        glValidateProgram(m_IDPrograma);

        // Liberación de los shaders individuales tras enlazarlos al programa principal
        glDetachShader(m_IDPrograma, sv);
        glDetachShader(m_IDPrograma, sf);
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

    // --- Métodos de Utilidad para Uniforms ---
    void EstablecerUniform1i(const std::string& nombre, int valor) const {
        glUniform1i(ObtenerUbicacionUniform(nombre), valor);
    }

    void EstablecerUniform1f(const std::string& nombre, float valor) const {
        glUniform1f(ObtenerUbicacionUniform(nombre), valor);
    }

    void EstablecerUniformVector3(const std::string& nombre, const Vector3& v) const {
        glUniform3f(ObtenerUbicacionUniform(nombre), v.X, v.Y, v.Z);
    }

    void EstablecerUniformMat4(const std::string& nombre, const float* matriz) const {
        glUniformMatrix4fv(ObtenerUbicacionUniform(nombre), 1, GL_FALSE, matriz);
    }

private:
    int ObtenerUbicacionUniform(const std::string& nombre) const {
        return glGetUniformLocation(m_IDPrograma, nombre.c_str());
    }
};
