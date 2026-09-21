#pragma once
#include <GL/gl.h>

#include "Entidad.hpp" 
#include "GestorEntidades.hpp"
#include "JerarquiaEscena.hpp"
#include "Inspector.hpp"
#include "ConsolaEditor.hpp"
#include "ControladorModo.hpp"
#include "SistemaRendimiento.hpp"
#include "SistemaSeleccion.hpp"
#include "SistemaComandos.hpp"
#include "VentanaFlotante.hpp" // <-- Nuestro gestor de ventanas nativo y móvil
#include "Entrada.hpp"          // <-- Necesario para obtener las coordenadas y estado del mouse
#include "EditorNodos.hpp"      // <-- Subsistema de programación visual por nodos

class SistemaUI {
private:
    bool m_MostrarEditor;
    bool m_MostrarDepuracionTensor; // <-- Toggle visual para depurar esfuerzos tensoriales en pantalla

    // Instancias de ventanas flotantes con sus posiciones y tamaños iniciales por defecto
    VentanaFlotante m_PanelJerarquia;
    VentanaFlotante m_PanelInspector;
    VentanaFlotante m_PanelConsola;
    VentanaFlotante m_PanelNodos;     // <-- Ventana flotante para el Editor de Nodos

    EditorNodos m_GestorNodos;      // <-- Instancia lógica del sistema de nodos

public:
    SistemaUI() 
        : m_MostrarEditor(true),
          m_MostrarDepuracionTensor(false),
          m_PanelJerarquia("Jerarquia", 10.0f, 45.0f, 270.0f, 475.0f),
          m_PanelInspector("Inspector", 1000.0f, 45.0f, 270.0f, 475.0f),
          m_PanelConsola("Consola de Diagnostico", 10.0f, 530.0f, 1260.0f, 180.0f),
          m_PanelNodos("Editor de Nodos (Visual Scripting)", 300.0f, 100.0f, 680.0f, 400.0f) {}

    void AlternarModoEditor() { m_MostrarEditor = !m_MostrarEditor; }
    bool EstaModoEditorActivo() const { return m_MostrarEditor; }

    void AlternarDepuracionTensor() { m_MostrarDepuracionTensor = !m_MostrarDepuracionTensor; }
    bool EstaDepuracionTensorActiva() const { return m_MostrarDepuracionTensor; }

    // Acceso al gestor de nodos para inicializar gráficos o esquemas lógicos
    EditorNodos& ObtenerGestorNodos() { return m_GestorNodos; }

    void DibujarPanelEditor(GestorEntidades& gestor, 
                            JerarquiaEscena& jerarquia, 
                            Inspector& inspector, 
                            const ControladorModo& controlador, 
                            SistemaRendimiento& rendimiento,
                            SistemaSeleccion& seleccion) 
    {
        if (!m_MostrarEditor) return;

        // Obtener estado del ratón para el arrastre de ventanas flotantes
        float mouseX = Entrada::ObtenerPosicionRatonX();
        float mouseY = Entrada::ObtenerPosicionRatonY();
        bool mousePresionado = Entrada::EstaPresionadoBotonRaton(0);

        // Actualizar lógica de arrastre de cada ventana flotante
        m_PanelJerarquia.ComprobadorInicioArrastre(mouseX, mouseY, mousePresionado);
        m_PanelJerarquia.ActualizarArrastre(mouseX, mouseY, mousePresionado);

        m_PanelInspector.ComprobadorInicioArrastre(mouseX, mouseY, mousePresionado);
        m_PanelInspector.ActualizarArrastre(mouseX, mouseY, mousePresionado);

        m_PanelConsola.ComprobadorInicioArrastre(mouseX, mouseY, mousePresionado);
        m_PanelConsola.ActualizarArrastre(mouseX, mouseY, mousePresionado);

        // Actualizar arrastre del panel de nodos (si está activo/visible)
        if (m_GestorNodos.EstaActivo()) {
            m_PanelNodos.ComprobadorInicioArrastre(mouseX, mouseY, mousePresionado);
            m_PanelNodos.ActualizarArrastre(mouseX, mouseY, mousePresionado);
        }

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, 1280, 720, 0, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glDisable(GL_DEPTH_TEST);

        // Barra Superior Estática del Editor
        glColor4f(0.04f, 0.04f, 0.06f, 0.90f);
        glBegin(GL_QUADS);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(1280.0f, 0.0f);
            glVertex2f(1280.0f, 35.0f);
            glVertex2f(0.0f, 35.0f);
        glEnd();

        // Botón PLAY / STOP indicador (Centro de la barra superior)
        if (controlador.EstaEnModoJuego()) {
            glColor3f(0.2f, 0.8f, 0.2f); // Verde (Play activo)
        } else {
            glColor3f(0.8f, 0.2f, 0.2f); // Rojo (Editor / Stop)
        }
        glBegin(GL_TRIANGLES);
            glVertex2f(630.0f, 10.0f);
            glVertex2f(630.0f, 25.0f);
            glVertex2f(645.0f, 17.5f);
        glEnd();

        // ==========================================
        // INDICADOR VISUAL DE ESTADO TENSORIAL EN BARRA SUPERIOR
        // ==========================================
        if (m_MostrarDepuracionTensor) {
            glColor3f(0.9f, 0.6f, 0.1f); // Naranja indicando modo tensor activo
        } else {
            glColor3f(0.3f, 0.3f, 0.4f); // Gris inactivo
        }
        glBegin(GL_QUADS);
            glVertex2f(700.0f, 10.0f);
            glVertex2f(725.0f, 10.0f);
            glVertex2f(725.0f, 25.0f);
            glVertex2f(700.0f, 25.0f);
        glEnd();

        // --- Dibujado Dinámico de Paneles Flotantes (Basados en sus coordenadas X, Y) ---

        // 1. Panel Jerarquía
        glColor4f(0.06f, 0.06f, 0.09f, 0.85f);
        glBegin(GL_QUADS);
            glVertex2f(m_PanelJerarquia.X, m_PanelJerarquia.Y);
            glVertex2f(m_PanelJerarquia.X + m_PanelJerarquia.Ancho, m_PanelJerarquia.Y);
            glVertex2f(m_PanelJerarquia.X + m_PanelJerarquia.Ancho, m_PanelJerarquia.Y + m_PanelJerarquia.Alto);
            glVertex2f(m_PanelJerarquia.X, m_PanelJerarquia.Y + m_PanelJerarquia.Alto);
        glEnd();
        // Barra de título interactiva de la Jerarquía
        glColor4f(0.12f, 0.12f, 0.18f, 0.95f);
        glBegin(GL_QUADS);
            glVertex2f(m_PanelJerarquia.X, m_PanelJerarquia.Y);
            glVertex2f(m_PanelJerarquia.X + m_PanelJerarquia.Ancho, m_PanelJerarquia.Y);
            glVertex2f(m_PanelJerarquia.X + m_PanelJerarquia.Ancho, m_PanelJerarquia.Y + 25.0f);
            glVertex2f(m_PanelJerarquia.X, m_PanelJerarquia.Y + 25.0f);
        glEnd();

        // 2. Panel Inspector
        glColor4f(0.06f, 0.06f, 0.09f, 0.85f);
        glBegin(GL_QUADS);
            glVertex2f(m_PanelInspector.X, m_PanelInspector.Y);
            glVertex2f(m_PanelInspector.X + m_PanelInspector.Ancho, m_PanelInspector.Y);
            glVertex2f(m_PanelInspector.X + m_PanelInspector.Ancho, m_PanelInspector.Y + m_PanelInspector.Alto);
            glVertex2f(m_PanelInspector.X, m_PanelInspector.Y + m_PanelInspector.Alto);
        glEnd();
        // Barra de título interactiva del Inspector
        glColor4f(0.12f, 0.12f, 0.18f, 0.95f);
        glBegin(GL_QUADS);
            glVertex2f(m_PanelInspector.X, m_PanelInspector.Y);
            glVertex2f(m_PanelInspector.X + m_PanelInspector.Ancho, m_PanelInspector.Y);
            glVertex2f(m_PanelInspector.X + m_PanelInspector.Ancho, m_PanelInspector.Y + 25.0f);
            glVertex2f(m_PanelInspector.X, m_PanelInspector.Y + 25.0f);
        glEnd();

        // 3. Panel Consola
        glColor4f(0.03f, 0.03f, 0.04f, 0.90f);
        glBegin(GL_QUADS);
            glVertex2f(m_PanelConsola.X, m_PanelConsola.Y);
            glVertex2f(m_PanelConsola.X + m_PanelConsola.Ancho, m_PanelConsola.Y);
            glVertex2f(m_PanelConsola.X + m_PanelConsola.Ancho, m_PanelConsola.Y + m_PanelConsola.Alto);
            glVertex2f(m_PanelConsola.X, m_PanelConsola.Y + m_PanelConsola.Alto);
        glEnd();
        // Barra de título interactiva de la Consola
        glColor4f(0.08f, 0.08f, 0.12f, 0.95f);
        glBegin(GL_QUADS);
            glVertex2f(m_PanelConsola.X, m_PanelConsola.Y);
            glVertex2f(m_PanelConsola.X + m_PanelConsola.Ancho, m_PanelConsola.Y);
            glVertex2f(m_PanelConsola.X + m_PanelConsola.Ancho, m_PanelConsola.Y + 25.0f);
            glVertex2f(m_PanelConsola.X, m_PanelConsola.Y + 25.0f);
        glEnd();

        // 4. Panel Editor de Nodos y su Gráfico Visual (Solo se dibuja si el usuario lo activa)
        if (m_GestorNodos.EstaActivo()) {
            glColor4f(0.05f, 0.05f, 0.07f, 0.92f); // Fondo oscuro estilo grafo de nodos
            glBegin(GL_QUADS);
                glVertex2f(m_PanelNodos.X, m_PanelNodos.Y);
                glVertex2f(m_PanelNodos.X + m_PanelNodos.Ancho, m_PanelNodos.Y);
                glVertex2f(m_PanelNodos.X + m_PanelNodos.Ancho, m_PanelNodos.Y + m_PanelNodos.Alto);
                glVertex2f(m_PanelNodos.X, m_PanelNodos.Y + m_PanelNodos.Alto);
            glEnd();
            // Barra de título interactiva del Editor de Nodos
            glColor4f(0.10f, 0.15f, 0.22f, 0.95f);
            glBegin(GL_QUADS);
                glVertex2f(m_PanelNodos.X, m_PanelNodos.Y);
                glVertex2f(m_PanelNodos.X + m_PanelNodos.Ancho, m_PanelNodos.Y);
                glVertex2f(m_PanelNodos.X + m_PanelNodos.Ancho, m_PanelNodos.Y + 25.0f);
                glVertex2f(m_PanelNodos.X, m_PanelNodos.Y + 25.0f);
            glEnd();

            // Renderizado de los bloques de nodos, pines y conexiones dentro de los límites del panel
            m_GestorNodos.RenderizarGrafoNodos(m_PanelNodos.X, m_PanelNodos.Y + 25.0f);
        }

        // Restaurar estado de profundidad antes de procesar lógica de módulos secundarios
        glEnable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        // ==========================================
        // ENLACE LÓGICO DE LOS MÓDULOS DE INTERFAZ
        // ==========================================
        
        // Renderizado del árbol de jerarquía de la escena
        jerarquia.RenderizarArbol(gestor);
        
        // Inspector con la entidad seleccionada real
        inspector.MostrarInspector(gestor, seleccion.ObtenerEntidadSeleccionada());

        // Procesamiento y sincronización de registros de la Consola del Editor
        const auto& logsConsola = ConsolaEditor::ObtenerInstancia().ObtenerLogs();

        // Sistema universal de rendimiento y monitoreo
        rendimiento.RenderizarPanelRendimiento();
    }
};
