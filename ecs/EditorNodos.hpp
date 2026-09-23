#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <GL/gl.h>
#include "Entidad.hpp"
#include "Componentes.hpp"
#include "GestorEntidades.hpp"

struct PinNodo {
    std::string Nombre;
    bool EsEntrada; // true = input pin, false = output pin
};

struct NodoVisual {
    int ID;
    std::string Titulo;
    float X, Y; // Posición en pantalla del nodo
    float Ancho, Alto;
    std::vector<PinNodo> Pines;
    std::string TipoAccion; // Ej: "InputKey", "Branch", "SetColor", "DebugLog", "AplicarFuerza", "ModificarTensor"
    
    // Parámetros internos configurables para la ejecución del nodo
    float ParametroFloat = 0.0f;
    std::string ParametroTexto = "";
};

struct ConexionNodos {
    int NodoOrigenID;
    int PinOrigenIndex;
    int NodoDestinoID;
    int PinDestinoIndex;
};

class EditorNodos {
private:
    std::vector<NodoVisual> m_Nodos;
    std::vector<ConexionNodos> m_Conexiones;
    int m_SiguienteID = 1;
    bool m_MostrarEditorNodos = false;

public:
    void AlternarEditorNodos() {
        m_MostrarEditorNodos = !m_MostrarEditorNodos;
    }

    bool EstaActivo() const {
        return m_MostrarEditorNodos;
    }

    int CrearNodo(const std::string& titulo, float x, float y, const std::string& tipo) {
        NodoVisual nuevoNodo;
        nuevoNodo.ID = m_SiguienteID++;
        nuevoNodo.Titulo = titulo;
        nuevoNodo.X = x;
        nuevoNodo.Y = y;
        nuevoNodo.Ancho = 180.0f; // Ancho visual por defecto del nodo
        nuevoNodo.Alto = 90.0f;   // Alto dinámico según pines
        nuevoNodo.TipoAccion = tipo;
        m_Nodos.push_back(nuevoNodo);
        
        std::cout << "[EditorNodos] Nodo creado: " << titulo << " (ID: " << nuevoNodo.ID << ")\n";
        return nuevoNodo.ID;
    }

    void AgregarPin(int nodoID, const std::string& nombrePin, bool esEntrada) {
        for (auto& nodo : m_Nodos) {
            if (nodo.ID == nodoID) {
                nodo.Pines.push_back({ nombrePin, esEntrada });
                nodo.Alto += 18.0f; // Expandir el tamaño del nodo por cada pin añadido
                break;
            }
        }
    }

    void ConectarPines(int origenID, int pinOrigenIdx, int destinoID, int pinDestinoIdx) {
        m_Conexiones.push_back({ origenID, pinOrigenIdx, destinoID, pinDestinoIdx });
        std::cout << "[EditorNodos] Conexión establecida entre nodo " << origenID << " y nodo " << destinoID << "\n";
    }

    const std::vector<NodoVisual>& ObtenerNodos() const {
        return m_Nodos;
    }

    const std::vector<ConexionNodos>& ObtenerConexiones() const {
        return m_Conexiones;
    }

    void LimpiarRed() {
        m_Nodos.clear();
        m_Conexiones.clear();
        m_SiguienteID = 1;
        std::cout << "[EditorNodos] Red de nodos limpiada.\n";
    }

    // =========================================================================
    // PUENTE DE EJECUCIÓN ECS: Traduce y ejecuta la red lógica sobre las entidades
    // =========================================================================
    void EjecutarRedNodos(GestorEntidades& gestor, const Entidad& entidadObjetivo) {
        if (!m_MostrarEditorNodos || m_Nodos.empty()) return;

        // Recorremos los nodos configurados en la red visual para evaluar su comportamiento
        for (const auto& nodo : m_Nodos) {
            if (nodo.TipoAccion == "DebugLog") {
                std::cout << "[ScriptVisual - DebugLog] Ejecutando nodo ID " << nodo.ID << ": " << nodo.ParametroTexto << "\n";
            }
            else if (nodo.TipoAccion == "AplicarFuerza") {
                // Interactúa directamente con el componente de cuerpo rígido del ECS
                ComponenteCuerpoRigido* cuerpo = gestor.ObtenerCuerpoRigido(entidadObjetivo);
                if (cuerpo) {
                    cuerpo->Velocidad = cuerpo->Velocidad + Vector3(0.0f, nodo.ParametroFloat, 0.0f);
                    std::cout << "[ScriptVisual] Fuerza aplicada vía nodos a entidad objetivo.\n";
                }
            }
            else if (nodo.TipoAccion == "ModificarTensor") {
                // Interactúa con el subsistema tensorial avanzado que integramos previamente
                ComponenteTensorDeformacion* tensor = gestor.ObtenerTensorDeformacion(entidadObjetivo);
                if (tensor) {
                    Vector3 impactoSimulado(nodo.ParametroFloat, nodo.ParametroFloat * 1.5f, 0.0f);
                    Vector3 normalSuelo(0.0f, 1.0f, 0.0f);
                    tensor->AplicarImpactoTensorial(impactoSimulado, normalSuelo);
                    std::cout << "[ScriptVisual] Impacto tensorial ejecutado desde el nodo visual.\n";
                }
            }
        }
    }

    // =========================================================================
    // RENDERIZADO GRÁFICO DE NODOS, PINES Y CONEXIONES (Estilo Blueprint / UI)
    // =========================================================================
    void RenderizarGrafoNodos(float offsetX, float offsetY) {
        if (!m_MostrarEditorNodos) return;

        // 1. Dibujar Cables de Conexión
        glLineWidth(2.5f);
        glBegin(GL_LINES);
        for (const auto& conexion : m_Conexiones) {
            const NodoVisual* nodoOri = nullptr;
            const NodoVisual* nodoDes = nullptr;

            for (const auto& n : m_Nodos) {
                if (n.ID == conexion.NodoOrigenID) nodoOri = &n;
                if (n.ID == conexion.NodoDestinoID) nodoDes = &n;
            }

            if (nodoOri && nodoDes) {
                float x1 = offsetX + nodoOri->X + nodoOri->Ancho;
                float y1 = offsetY + nodoOri->Y + (nodoOri->Alto * 0.5f);
                float x2 = offsetX + nodoDes->X;
                float y2 = offsetY + nodoDes->Y + (nodoDes->Alto * 0.5f);

                glColor3f(0.2f, 0.8f, 0.4f); // Verde ejecución
                glVertex2f(x1, y1);
                glVertex2f(x2, y2);
            }
        }
        glEnd();
        glLineWidth(1.0f);

        // 2. Dibujar Cada Nodo y sus Pines
        for (const auto& nodo : m_Nodos) {
            float rx = offsetX + nodo.X;
            float ry = offsetY + nodo.Y;

            // Cuerpo principal del nodo
            glColor4f(0.12f, 0.12f, 0.16f, 0.95f);
            glBegin(GL_QUADS);
                glVertex2f(rx, ry);
                glVertex2f(rx + nodo.Ancho, ry);
                glVertex2f(rx + nodo.Ancho, ry + nodo.Alto);
                glVertex2f(rx, ry + nodo.Alto);
            glEnd();

            // Barra de título superior del Nodo
            glColor4f(0.85f, 0.35f, 0.10f, 0.95f);
            glBegin(GL_QUADS);
                glVertex2f(rx, ry);
                glVertex2f(rx + nodo.Ancho, ry);
                glVertex2f(rx + nodo.Ancho, ry + 22.0f);
                glVertex2f(rx, ry + 22.0f);
            glEnd();

            // Contorno sutil del Nodo
            glColor4f(0.3f, 0.3f, 0.4f, 1.0f);
            glBegin(GL_LINE_LOOP);
                glVertex2f(rx, ry);
                glVertex2f(rx + nodo.Ancho, ry);
                glVertex2f(rx + nodo.Ancho, ry + nodo.Alto);
                glVertex2f(rx, ry + nodo.Alto);
            glEnd();

            // Dibujar Pines dentro del Nodo
            float pinYOffset = ry + 35.0f;
            for (const auto& pin : nodo.Pines) {
                if (pin.EsEntrada) {
                    glColor3f(0.2f, 0.7f, 0.9f); // Azul entrada
                } else {
                    glColor3f(0.9f, 0.9f, 0.2f); // Amarillo salida
                }

                float pinX = pin.EsEntrada ? rx + 8.0f : rx + nodo.Ancho - 12.0f;
                
                glBegin(GL_QUADS);
                    glVertex2f(pinX - 3.0f, pinYOffset - 3.0f);
                    glVertex2f(pinX + 3.0f, pinYOffset - 3.0f);
                    glVertex2f(pinX + 3.0f, pinYOffset + 3.0f);
                    glVertex2f(pinX - 3.0f, pinYOffset + 3.0f);
                glEnd();

                pinYOffset += 18.0f;
            }
        }
    }
};
