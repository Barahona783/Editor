#pragma once
#include <vector>
#include <cmath>
#include <iostream>
#include "Vector3.hpp"

// Clase que representa una Capa Individual de la Red Neuronal
class CapaNeuronal {
public:
    std::vector<std::vector<float>> Pesos; // Matriz de pesos W (Neuronas x Entradas)
    std::vector<float> Sesgos;             // Vector de sesgos (Bias)
    std::vector<float> Salidas;            // Activaciones resultantes (z o a)

    CapaNeuronal(int numNeuronas, int numEntradas) {
        Pesos.resize(numNeuronas, std::vector<float>(numEntradas, 0.5f)); // Inicialización base
        Sesgos.resize(numNeuronas, 0.0f);
        Salidas.resize(numNeuronas, 0.0f);
    }

    // Función de activación Sigmoide: σ(z) = 1 / (1 + e^-z)[span_1](start_span)[span_1](end_span)
    float ActivarSigmoide(float z) const {
        return 1.0f / (1.0f + std::exp(-z));
    }

    // Propagación hacia adelante (Forward Propagation): Z = W * X + b
    std::vector<float> Propagar(const std::vector<float>& entrada) {
        int numNeuronas = Pesos.size();
        int numEntradas = entrada.size();

        for (int i = 0; i < numNeuronas; ++i) {
            float suma = Sesgos[i];
            for (int j = 0; j < numEntradas; ++j) {
                suma += Pesos[i][j] * entrada[j];
            }
            Salidas[i] = ActivarSigmoide(suma);
        }
        return Salidas;
    }
};

// Sistema de Red Neuronal Multicapa para el Motor
class RedNeuronal {
private:
    std::vector<CapaNeuronal> m_Capas;

public:
    RedNeuronal() {}

    // Añade una capa a la red especificando cantidad de neuronas y entradas
    void AnadirCapa(int numNeuronas, int numEntradas) {
        m_Capas.emplace_back(numNeuronas, numEntradas);
    }

    // Ejecuta la inferencia completa desde la capa de entrada hasta la salida
    std::vector<float> Predecir(const std::vector<float>& datosEntrada) {
        if (m_Capas.empty()) return {};

        std::vector<float> activacionActual = m_Capas[0].Propagar(datosEntrada);
        
        for (size_t i = 1; i < m_Capas.size(); ++i) {
            activacionActual = m_Capas[i].Propagar(activacionActual);
        }

        return activacionActual;
    }
};

// Componente ECS para asociar una Red Neuronal a cualquier entidad del motor
struct ComponenteRedNeuronal {
    RedNeuronal Cerebro;
    bool Activo;

    ComponenteRedNeuronal() : Activo(true) {
        // Configuramos una arquitectura por defecto (Ej: 3 entradas, 4 neuronas ocultas, 2 salidas de decisión)
        Cerebro.AnadirCapa(4, 3); // Capa oculta 1
        Cerebro.AnadirCapa(2, 4); // Capa de salida
    }
};
