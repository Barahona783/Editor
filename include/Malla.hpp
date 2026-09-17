#pragma once
#include <vector>
#include <GL/gl.h>
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

    void InicializarEnGPU() {
        // Preparado para la inicialización de buffers gráficos
    }

    void Dibujar() const {
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, sizeof(Vertice), m_Vertices.data());
        
        if (!m_Indices.empty()) {
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_Indices.size()), GL_UNSIGNED_INT, m_Indices.data());
        } else {
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_Vertices.size()));
        }

        glDisableClientState(GL_VERTEX_ARRAY);
    }
};