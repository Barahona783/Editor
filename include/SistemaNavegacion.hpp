#pragma once
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <iostream>
#include "Entidad.hpp"
#include "Componentes.hpp"

// Estructura que representa un nodo de ruta en el mundo abierto (Calles/Aceras)
struct NodoRuta {
    int ID;
    float X, Y, Z;
    std::vector<int> ConexionesIDs; // IDs de los nodos adyacentes a los que se puede viajar
};

// Componente asignado a entidades móviles (NPCs o Vehículos) para que sigan una ruta
struct ComponenteNavegacionAgente {
    int NodoActualID = -1;
    int NodoDestinoID = -1;
    float VelocidadMovimiento = 3.5f;
    float RadioLlegada = 1.5f;
    bool EnMovimiento = false;
};

// Componente para identificar edificios con puntos de entrada y salida lógicos
struct ComponenteEdificioInteractivo {
    std::string NombreEdificio;
    Vector3 PuertaEntrada; // Posición exacta de acceso exterior
    Vector3 PuntoInterior; // Posición de llegada al entrar al edificio
    bool EstaAbierto = true;
};

class SistemaNavegacion {
private:
    std::vector<NodoRuta> m_RedNodos;

public:
    // Añade un nodo de ruta a la red global del mapa
    int AgregarNodoRuta(float x, float y, float z) {
        NodoRuta nodo;
        nodo.ID = static_cast<int>(m_RedNodos.size()) + 1;
        nodo.X = x;
        nodo.Y = y;
        nodo.Z = z;
        m_RedNodos.push_back(nodo);
        return nodo.ID;
    }

    // Conecta bidireccionalmente dos nodos de ruta
    void ConectarNodos(int id1, int id2) {
        for (auto& nodo : m_RedNodos) {
            if (nodo.ID == id1) {
                if (std::find(nodo.ConexionesIDs.begin(), nodo.ConexionesIDs.end(), id2) == nodo.ConexionesIDs.end()) {
                    nodo.ConexionesIDs.push_back(id2);
                }
            } else if (nodo.ID == id2) {
                if (std::find(nodo.ConexionesIDs.begin(), nodo.ConexionesIDs.end(), id1) == nodo.ConexionesIDs.end()) {
                    nodo.ConexionesIDs.push_back(id1);
                }
            }
        }
    }

    // Encuentra el nodo de ruta más cercano a una posición en el mundo
    int EncontrarNodoMasCercano(float x, float z) const {
        if (m_RedNodos.empty()) return -1;

        int mejorID = m_RedNodos[0].ID;
        float menorDistanciaSq = -1.0f;

        for (const auto& nodo : m_RedNodos) {
            float dx = nodo.X - x;
            float dz = nodo.Z - z;
            float distanciaSq = (dx * dx) + (dz * dz);

            if (menorDistanciaSq < 0.0f || distanciaSq < menorDistanciaSq) {
                menorDistanciaSq = distanciaSq;
                mejorID = nodo.ID;
            }
        }
        return mejorID;
    }

    // Actualiza el desplazamiento de los agentes (NPCs/Vehículos) a lo largo del grafo de nodos
    void Actualizar(GestorEntidades& gestor, float deltaTime) {
        // Obtenemos todas las entidades para procesar las que tengan navegación y transformación
        const auto& entidades = gestor.ObtenerTodasLasEntidades();

        for (const auto& entidad : entidades) {
            ComponenteTransformacion* trans = gestor.ObtenerTransformacion(entidad);
            // Simulación de evaluación de movimiento vectorial hacia el nodo destino
            if (trans && !m_RedNodos.empty()) {
                // Lógica de avance guiado por el grafo de navegación
            }
        }
    }

    const std::vector<NodoRuta>& ObtenerRedNodos() const {
        return m_RedNodos;
    }
};
