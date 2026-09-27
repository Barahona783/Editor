#pragma once
#include <vector>
#include <GL/gl.h>
#include <iostream>
#include "Vector3.hpp"

struct Vertice {
    Vector3 Posicion;
};

class Malla {
private:
    std::vector<Vertice> m_Vertices;
    std::vector<unsigned int> m_Indices;
    unsigned int m_VAO, m_VBO, m_EBO;

public:
    Malla(const std::vector<Vertice>& vertices, const std::vector<unsigned int>& indices)
        : m_Vertices(vertices), m_Indices(indices), m_VAO(0), m_VBO(0), m_EBO(0) {}

    // Destructor para garantizar la liberación automática de recursos en la GPU (Cero Fugas)
    ~Malla() {
        LiberarEnGPU();
    }

    // Eliminar copia para prevenir doble liberación de identificadores de OpenGL en la GPU
    Malla(const Malla&) = delete;
    Malla& operator=(const Malla&) = delete;

    // Permitir movimiento si es necesario
    Malla(Malla&& otro) noexcept 
        : m_Vertices(std::move(otro.m_Vertices)), 
          m_Indices(std::move(otro.m_Indices)), 
          m_VAO(otro.m_VAO), 
          m_VBO(otro.m_VBO), 
          m_EBO(otro.m_EBO) {
        otro.m_VAO = 0;
        otro.m_VBO = 0;
        otro.m_EBO = 0;
    }

    Malla& operator=(Malla&& otro) noexcept {
        if (this != &otro) {
            LiberarEnGPU();
            m_Vertices = std::move(otro.m_Vertices);
            m_Indices = std::move(otro.m_Indices);
            m_VAO = otro.m_VAO;
            m_VBO = otro.m_VBO;
            m_EBO = otro.m_EBO;
            otro.m_VAO = 0;
            otro.m_VBO = 0;
            otro.m_EBO = 0;
        }
        return *this;
    }

    void InicializarEnGPU() {
        if (m_Vertices.empty()) return;

        // Generar VAO y VBO
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);

        // Cargar datos de vértices en el VBO
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertice), m_Vertices.data(), GL_STATIC_DRAW);

        // Si existen índices, configurar y cargar el EBO
        if (!m_Indices.empty()) {
            glGenBuffers(1, &m_EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), m_Indices.data(), GL_STATIC_DRAW);
        }

        // Configurar el puntero del atributo de posición (layout location = 0)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertice), (void*)0);

        // Desvincular el VAO para dejar el estado limpio
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        if (!m_Indices.empty()) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
    }

    void Dibujar() const {
        if (m_VAO == 0) return;

        glBindVertexArray(m_VAO);
        
        if (!m_Indices.empty()) {
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_Indices.size()), GL_UNSIGNED_INT, 0);
        } else {
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_Vertices.size()));
        }

        glBindVertexArray(0);
    }

    void LiberarEnGPU() {
        if (m_VAO) {
            glDeleteVertexArrays(1, &m_VAO);
            m_VAO = 0;
        }
        if (m_VBO) {
            glDeleteBuffers(1, &m_VBO);
            m_VBO = 0;
        }
        if (m_EBO) {
            glDeleteBuffers(1, &m_EBO);
            m_EBO = 0;
        }
    }
};
