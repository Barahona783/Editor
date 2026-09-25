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
#include "VentanaFlotante.hpp"
#include "Entrada.hpp"
#include "EditorNodos.hpp"
#include "CirculoMohr.hpp"

class SistemaUI {
private:
    bool m_MostrarEditor;
    bool m_MostrarDepuracionTensor;
    bool m_MostrarDepuracionMundo; 
    bool m_MostrarDepuracionOpticaAnim; // <-- NUEVO: Control para visualizar el panel unificado de Óptica y Animación

    VentanaFlotante m_PanelJerarquia;
    VentanaFlotante m_PanelInspector;
    VentanaFlotante m_PanelConsola;
    VentanaFlotante m_PanelNodos;
    VentanaFlotante m_PanelTensor;
    VentanaFlotante m_PanelMundo; 
    VentanaFlotante m_PanelOpticaAnim; // <-- NUEVO: Panel flotante para línea de tiempo y óptica (Ley de Fermat)

    EditorNodos m_GestorNodos;

public:
    SistemaUI() 
        : m_MostrarEditor(true),
          m_MostrarDepuracionTensor(false),
          m_MostrarDepuracionMundo(false),
          m_MostrarDepuracionOpticaAnim(false),
          m_PanelJerarquia("Jerarquia", 10.0f, 45.0f, 270.0f, 475.0f),
          m_PanelInspector("Inspector", 1000.0f, 45.0f, 270.0f, 475.0f),
          m_PanelConsola("Consola de Diagnostico", 10.0f, 530.0f, 1260.0f, 180.0f),
          m_PanelNodos("Editor de Nodos (Visual Scripting)", 300.0f, 100.0f, 680.0f, 400.0f),
          m_PanelTensor("Analisis Tensorial (Circulo de Mohr)", 310.0f, 120.0f, 420.0f, 320.0f),
          m_PanelMundo("Conciencia Espacial y Navegacion", 310.0f, 450.0f, 420.0f, 220.0f),
          m_PanelOpticaAnim("Optica (Fermat) y Animacion Keyframes", 740.0f, 120.0f, 240.0f, 220.0f) {}

    void AlternarModoEditor() { m_MostrarEditor = !m_MostrarEditor; }
    bool EstaModoEditorActivo() const { return m_MostrarEditor; }

    void AlternarDepuracionTensor() { m_MostrarDepuracionTensor = !m_MostrarDepuracionTensor; }
    bool EstaDepuracionTensorActiva() const { return m_MostrarDepuracionTensor; }

    void AlternarDepuracionMundo() { m_MostrarDepuracionMundo = !m_MostrarDepuracionMundo; }
    bool EstaDepuracionMundoActiva() const { return m_MostrarDepuracionMundo; }

    void AlternarDepuracionOpticaAnim() { m_MostrarDepuracionOpticaAnim = !m_MostrarDepuracionOpticaAnim; }
    bool EstaDepuracionOpticaAnimActiva() const { return m_MostrarDepuracionOpticaAnim; }

    EditorNodos& ObtenerGestorNodos() { return m_GestorNodos; }

    void DibujarPanelEditor(GestorEntidades& gestor, 
                            JerarquiaEscena& jerarquia, 
                            Inspector& inspector, 
                            const ControladorModo& controlador, 
                            SistemaRendimiento& rendimiento,
                            SistemaSeleccion& seleccion) 
    {
        if (!m_MostrarEditor) return;

        float mouseX = Entrada::ObtenerMouseX();
        float mouseY = Entrada::ObtenerMouseY();
        bool mousePresionado = Entrada::BotonPresionado(0);

        // --- Detección de clics en los botones superiores de la barra de herramientas ---
        // 1. Botón Círculo de Mohr (X: 700-725, Y: 10-25)
        static bool clicAnteriorTensor = false;
        if (mousePresionado && !clicAnteriorTensor) {
            if (mouseX >= 700.0f && mouseX <= 725.0f && mouseY >= 10.0f && mouseY <= 25.0f) {
                AlternarDepuracionTensor();
            }
        }
        clicAnteriorTensor = mousePresionado;

        // 2. Botón Depuración de Mundo/Navegación (X: 740-765, Y: 10-25)
        static bool clicAnteriorMundo = false;
        if (mousePresionado && !clicAnteriorMundo) {
            if (mouseX >= 740.0f && mouseX <= 765.0f && mouseY >= 10.0f && mouseY <= 25.0f) {
                AlternarDepuracionMundo();
            }
        }
        clicAnteriorMundo = mousePresionado;

        // 3. NUEVO: Botón Óptica y Animación Keyframes (X: 780-805, Y: 10-25)
        static bool clicAnteriorOpticaAnim = false;
        if (mousePresionado && !clicAnteriorOpticaAnim) {
            if (mouseX >= 780.0f && mouseX <= 805.0f && mouseY >= 10.0f && mouseY <= 25.0f) {
                AlternarDepuracionOpticaAnim();
            }
        }
        clicAnteriorOpticaAnim = mousePresionado;

        m_PanelJerarquia.ComprobarInicioArrastre(mouseX, mouseY, mousePresionado);
        m_PanelJerarquia.ActualizarArrastre(mouseX, mouseY, mousePresionado);

        m_PanelInspector.ComprobarInicioArrastre(mouseX, mouseY, mousePresionado);
        m_PanelInspector.ActualizarArrastre(mouseX, mouseY, mousePresionado);

        m_PanelConsola.ComprobarInicioArrastre(mouseX, mouseY, mousePresionado);
        m_PanelConsola.ActualizarArrastre(mouseX, mouseY, mousePresionado);

        if (m_GestorNodos.EstaActivo()) {
            m_PanelNodos.ComprobarInicioArrastre(mouseX, mouseY, mousePresionado);
            m_PanelNodos.ActualizarArrastre(mouseX, mouseY, mousePresionado);

            if (controlador.EstaEnModoJuego()) {
                Entidad entidadActual = seleccion.ObtenerEntidadSeleccionada();
                if (entidadActual.EsValida()) {
                    m_GestorNodos.EjecutarRedNodos(gestor, entidadActual);
                }
            }
        }

        if (m_MostrarDepuracionTensor) {
            m_PanelTensor.ComprobarInicioArrastre(mouseX, mouseY, mousePresionado);
            m_PanelTensor.ActualizarArrastre(mouseX, mouseY, mousePresionado);
        }

        if (m_MostrarDepuracionMundo) {
            m_PanelMundo.ComprobarInicioArrastre(mouseX, mouseY, mousePresionado);
            m_PanelMundo.ActualizarArrastre(mouseX, mouseY, mousePresionado);
        }

        if (m_MostrarDepuracionOpticaAnim) {
            m_PanelOpticaAnim.ComprobarInicioArrastre(mouseX, mouseY, mousePresionado);
            m_PanelOpticaAnim.ActualizarArrastre(mouseX, mouseY, mousePresionado);
        }

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, 1280, 720, 0, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glDisable(GL_DEPTH_TEST);

        // Barra superior del editor
        glColor4f(0.04f, 0.04f, 0.06f, 0.90f);
        glBegin(GL_QUADS);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(1280.0f, 0.0f);
            glVertex2f(1280.0f, 35.0f);
            glVertex2f(0.0f, 35.0f);
        glEnd();

        // Botón Play / Modo Juego (Triángulo)
        if (controlador.EstaEnModoJuego()) {
            glColor3f(0.2f, 0.8f, 0.2f);
        } else {
            glColor3f(0.8f, 0.2f, 0.2f);
        }
        glBegin(GL_TRIANGLES);
            glVertex2f(630.0f, 10.0f);
            glVertex2f(630.0f, 25.0f);
            glVertex2f(645.0f, 17.5f);
        glEnd();

        // Indicador de Tensor (Cuadrado Naranja)
        if (m_MostrarDepuracionTensor) {
            glColor3f(0.9f, 0.6f, 0.1f);
        } else {
            glColor3f(0.3f, 0.3f, 0.4f);
        }
        glBegin(GL_QUADS);
            glVertex2f(700.0f, 10.0f);
            glVertex2f(725.0f, 10.0f);
            glVertex2f(725.0f, 25.0f);
            glVertex2f(700.0f, 25.0f);
        glEnd();

        // Indicador de Mundo/Navegación (Cuadrado Cian)
        if (m_MostrarDepuracionMundo) {
            glColor3f(0.2f, 0.7f, 0.9f);
        } else {
            glColor3f(0.3f, 0.3f, 0.4f);
        }
        glBegin(GL_QUADS);
            glVertex2f(740.0f, 10.0f);
            glVertex2f(765.0f, 10.0f);
            glVertex2f(765.0f, 25.0f);
            glVertex2f(740.0f, 25.0f);
        glEnd();

        // NUEVO: Indicador de Óptica y Animación (Cuadrado Morado/Magenta)
        if (m_MostrarDepuracionOpticaAnim) {
            glColor3f(0.7f, 0.2f, 0.9f);
        } else {
            glColor3f(0.3f, 0.3f, 0.4f);
        }
        glBegin(GL_QUADS);
            glVertex2f(780.0f, 10.0f);
            glVertex2f(805.0f, 10.0f);
            glVertex2f(805.0f, 25.0f);
            glVertex2f(780.0f, 25.0f);
        glEnd();

        // Panel Jerarquia
        glColor4f(0.06f, 0.06f, 0.09f, 0.85f);
        glBegin(GL_QUADS);
            glVertex2f(m_PanelJerarquia.X, m_PanelJerarquia.Y);
            glVertex2f(m_PanelJerarquia.X + m_PanelJerarquia.Ancho, m_PanelJerarquia.Y);
            glVertex2f(m_PanelJerarquia.X + m_PanelJerarquia.Ancho, m_PanelJerarquia.Y + m_PanelJerarquia.Alto);
            glVertex2f(m_PanelJerarquia.X, m_PanelJerarquia.Y + m_PanelJerarquia.Alto);
        glEnd();
        glColor4f(0.12f, 0.12f, 0.18f, 0.95f);
        glBegin(GL_QUADS);
            glVertex2f(m_PanelJerarquia.X, m_PanelJerarquia.Y);
            glVertex2f(m_PanelJerarquia.X + m_PanelJerarquia.Ancho, m_PanelJerarquia.Y);
            glVertex2f(m_PanelJerarquia.X + m_PanelJerarquia.Ancho, m_PanelJerarquia.Y + 25.0f);
            glVertex2f(m_PanelJerarquia.X, m_PanelJerarquia.Y + 25.0f);
        glEnd();

        // Panel Inspector
        glColor4f(0.06f, 0.06f, 0.09f, 0.85f);
        glBegin(GL_QUADS);
            glVertex2f(m_PanelInspector.X, m_PanelInspector.Y);
            glVertex2f(m_PanelInspector.X + m_PanelInspector.Ancho, m_PanelInspector.Y);
            glVertex2f(m_PanelInspector.X + m_PanelInspector.Ancho, m_PanelInspector.Y + m_PanelInspector.Alto);
            glVertex2f(m_PanelInspector.X, m_PanelInspector.Y + m_PanelInspector.Alto);
        glEnd();
        glColor4f(0.12f, 0.12f, 0.18f, 0.95f);
        glBegin(GL_QUADS);
            glVertex2f(m_PanelInspector.X, m_PanelInspector.Y);
            glVertex2f(m_PanelInspector.X + m_PanelInspector.Ancho, m_PanelInspector.Y);
            glVertex2f(m_PanelInspector.X + m_PanelInspector.Ancho, m_PanelInspector.Y + 25.0f);
            glVertex2f(m_PanelInspector.X, m_PanelInspector.Y + 25.0f);
        glEnd();

        // Panel Consola
        glColor4f(0.03f, 0.03f, 0.04f, 0.90f);
        glBegin(GL_QUADS);
            glVertex2f(m_PanelConsola.X, m_PanelConsola.Y);
            glVertex2f(m_PanelConsola.X + m_PanelConsola.Ancho, m_PanelConsola.Y);
            glVertex2f(m_PanelConsola.X + m_PanelConsola.Ancho, m_PanelConsola.Y + m_PanelConsola.Alto);
            glVertex2f(m_PanelConsola.X, m_PanelConsola.Y + m_PanelConsola.Alto);
        glEnd();
        glColor4f(0.08f, 0.08f, 0.12f, 0.95f);
        glBegin(GL_QUADS);
            glVertex2f(m_PanelConsola.X, m_PanelConsola.Y);
            glVertex2f(m_PanelConsola.X + m_PanelConsola.Ancho, m_PanelConsola.Y);
            glVertex2f(m_PanelConsola.X + m_PanelConsola.Ancho, m_PanelConsola.Y + 25.0f);
            glVertex2f(m_PanelConsola.X, m_PanelConsola.Y + 25.0f);
        glEnd();

        // Panel Editor de Nodos
        if (m_GestorNodos.EstaActivo()) {
            glColor4f(0.05f, 0.05f, 0.07f, 0.92f);
            glBegin(GL_QUADS);
                glVertex2f(m_PanelNodos.X, m_PanelNodos.Y);
                glVertex2f(m_PanelNodos.X + m_PanelNodos.Ancho, m_PanelNodos.Y);
                glVertex2f(m_PanelNodos.X + m_PanelNodos.Ancho, m_PanelNodos.Y + m_PanelNodos.Alto);
                glVertex2f(m_PanelNodos.X, m_PanelNodos.Y + m_PanelNodos.Alto);
            glEnd();
            glColor4f(0.10f, 0.15f, 0.22f, 0.95f);
            glBegin(GL_QUADS);
                glVertex2f(m_PanelNodos.X, m_PanelNodos.Y);
                glVertex2f(m_PanelNodos.X + m_PanelNodos.Ancho, m_PanelNodos.Y);
                glVertex2f(m_PanelNodos.X + m_PanelNodos.Ancho, m_PanelNodos.Y + 25.0f);
                glVertex2f(m_PanelNodos.X, m_PanelNodos.Y + 25.0f);
            glEnd();
            m_GestorNodos.RenderizarGrafoNodos(m_PanelNodos.X, m_PanelNodos.Y + 25.0f);
        }

        // Panel de Análisis Tensorial (Círculo de Mohr)
        if (m_MostrarDepuracionTensor) {
            glColor4f(0.07f, 0.07f, 0.10f, 0.92f);
            glBegin(GL_QUADS);
                glVertex2f(m_PanelTensor.X, m_PanelTensor.Y);
                glVertex2f(m_PanelTensor.X + m_PanelTensor.Ancho, m_PanelTensor.Y);
                glVertex2f(m_PanelTensor.X + m_PanelTensor.Ancho, m_PanelTensor.Y + m_PanelTensor.Alto);
                glVertex2f(m_PanelTensor.X, m_PanelTensor.Y + m_PanelTensor.Alto);
            glEnd();
            
            glColor4f(0.18f, 0.12f, 0.08f, 0.95f);
            glBegin(GL_QUADS);
                glVertex2f(m_PanelTensor.X, m_PanelTensor.Y);
                glVertex2f(m_PanelTensor.X + m_PanelTensor.Ancho, m_PanelTensor.Y);
                glVertex2f(m_PanelTensor.X + m_PanelTensor.Ancho, m_PanelTensor.Y + 25.0f);
                glVertex2f(m_PanelTensor.X, m_PanelTensor.Y + 25.0f);
            glEnd();

            EstadoEsfuerzos estadoActual = { -10.0f, 50.0f, 40.0f };
            ResultadosMohr res = CirculoMohr::Calcular(estadoActual);

            float centroVisualX = m_PanelTensor.X + m_PanelTensor.Ancho * 0.5f;
            float centroVisualY = m_PanelTensor.Y + m_PanelTensor.Alto * 0.55f;
            
            glColor3f(0.3f, 0.3f, 0.4f);
            glBegin(GL_LINES);
                glVertex2f(m_PanelTensor.X + 20.0f, centroVisualY);
                glVertex2f(m_PanelTensor.X + m_PanelTensor.Ancho - 20.0f, centroVisualY);
                glVertex2f(centroVisualX, m_PanelTensor.Y + 35.0f);
                glVertex2f(centroVisualX, m_PanelTensor.Y + m_PanelTensor.Alto - 15.0f);
            glEnd();

            glColor3f(0.9f, 0.7f, 0.2f);
            glBegin(GL_LINE_LOOP);
            int segmentos = 30;
            float radioVisual = 60.0f;
            for (int i = 0; i < segmentos; ++i) {
                float theta = 2.0f * 3.14159265f * float(i) / float(segmentos);
                float vx = centroVisualX + radioVisual * std::cos(theta);
                float vy = centroVisualY + radioVisual * std::sin(theta);
                glVertex2f(vx, vy);
            }
            glEnd();
        }

        // Panel de Conciencia Espacial y Navegación (Waypoints y Edificios)
        if (m_MostrarDepuracionMundo) {
            glColor4f(0.06f, 0.08f, 0.10f, 0.92f);
            glBegin(GL_QUADS);
                glVertex2f(m_PanelMundo.X, m_PanelMundo.Y);
                glVertex2f(m_PanelMundo.X + m_PanelMundo.Ancho, m_PanelMundo.Y);
                glVertex2f(m_PanelMundo.X + m_PanelMundo.Ancho, m_PanelMundo.Y + m_PanelMundo.Alto);
                glVertex2f(m_PanelMundo.X, m_PanelMundo.Y + m_PanelMundo.Alto);
            glEnd();
            
            glColor4f(0.10f, 0.25f, 0.35f, 0.95f);
            glBegin(GL_QUADS);
                glVertex2f(m_PanelMundo.X, m_PanelMundo.Y);
                glVertex2f(m_PanelMundo.X + m_PanelMundo.Ancho, m_PanelMundo.Y);
                glVertex2f(m_PanelMundo.X + m_PanelMundo.Ancho, m_PanelMundo.Y + 25.0f);
                glVertex2f(m_PanelMundo.X, m_PanelMundo.Y + 25.0f);
            glEnd();

            float gridX = m_PanelMundo.X + 20.0f;
            float gridY = m_PanelMundo.Y + 45.0f;
            glColor3f(0.2f, 0.6f, 0.8f);
            glBegin(GL_LINES);
                glVertex2f(gridX, gridY + 50.0f); glVertex2f(gridX + 120.0f, gridY + 50.0f);
                glVertex2f(gridX + 60.0f, gridY); glVertex2f(gridX + 60.0f, gridY + 100.0f);
            glEnd();
            
            glColor3f(0.9f, 0.9f, 0.2f);
            glBegin(GL_QUADS);
                glVertex2f(gridX + 55.0f, gridY + 45.0f);
                glVertex2f(gridX + 65.0f, gridY + 45.0f);
                glVertex2f(gridX + 65.0f, gridY + 55.0f);
                glVertex2f(gridX + 55.0f, gridY + 55.0f);
            glEnd();
        }

        // --- NUEVO: Panel de Óptica (Ley de Fermat) y Animación por Keyframes ---
        if (m_MostrarDepuracionOpticaAnim) {
            glColor4f(0.08f, 0.05f, 0.10f, 0.92f);
            glBegin(GL_QUADS);
                glVertex2f(m_PanelOpticaAnim.X, m_PanelOpticaAnim.Y);
                glVertex2f(m_PanelOpticaAnim.X + m_PanelOpticaAnim.Ancho, m_PanelOpticaAnim.Y);
                glVertex2f(m_PanelOpticaAnim.X + m_PanelOpticaAnim.Ancho, m_PanelOpticaAnim.Y + m_PanelOpticaAnim.Alto);
                glVertex2f(m_PanelOpticaAnim.X, m_PanelOpticaAnim.Y + m_PanelOpticaAnim.Alto);
            glEnd();
            
            glColor4f(0.30f, 0.10f, 0.40f, 0.95f);
            glBegin(GL_QUADS);
                glVertex2f(m_PanelOpticaAnim.X, m_PanelOpticaAnim.Y);
                glVertex2f(m_PanelOpticaAnim.X + m_PanelOpticaAnim.Ancho, m_PanelOpticaAnim.Y);
                glVertex2f(m_PanelOpticaAnim.X + m_PanelOpticaAnim.Ancho, m_PanelOpticaAnim.Y + 25.0f);
                glVertex2f(m_PanelOpticaAnim.X, m_PanelOpticaAnim.Y + 25.0f);
            glEnd();

            // Dibujo esquemático de línea de tiempo de keyframes y curva de refracción óptica
            float timelineX = m_PanelOpticaAnim.X + 20.0f;
            float timelineY = m_PanelOpticaAnim.Y + 50.0f;
            
            glColor3f(0.8f, 0.4f, 0.9f);
            glBegin(GL_LINES);
                // Eje de tiempo
                glVertex2f(timelineX, timelineY + 40.0f); 
                glVertex2f(timelineX + 200.0f, timelineY + 40.0f);
            glEnd();

            // Fotogramas clave simulados (Keyframes)
            glColor3f(1.0f, 0.8f, 0.2f);
            glBegin(GL_QUADS);
                glVertex2f(timelineX + 20.0f,  timelineY + 35.0f); glVertex2f(timelineX + 26.0f,  timelineY + 35.0f); glVertex2f(timelineX + 26.0f,  timelineY + 45.0f); glVertex2f(timelineX + 20.0f,  timelineY + 45.0f);
                glVertex2f(timelineX + 90.0f,  timelineY + 35.0f); glVertex2f(timelineX + 96.0f,  timelineY + 35.0f); glVertex2f(timelineX + 96.0f,  timelineY + 45.0f); glVertex2f(timelineX + 90.0f,  timelineY + 45.0f);
                glVertex2f(timelineX + 170.0f, timelineY + 35.0f); glVertex2f(timelineX + 176.0f, timelineY + 35.0f); glVertex2f(timelineX + 176.0f, timelineY + 45.0f); glVertex2f(timelineX + 170.0f, timelineY + 45.0f);
            glEnd();
        }

        glEnable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        jerarquia.RenderizarArbol(gestor);
        inspector.MostrarInspector(gestor, seleccion.ObtenerEntidadSeleccionada());
        rendimiento.RenderizarPanelRendimiento();
    }
};
