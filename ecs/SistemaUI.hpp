#pragma once
#include <GL/gl.h>

#include "Entidad.hpp" 
#include "GestorEntidades.hpp"
#include "JerarquiaEscena.hpp"
#include "Inspector.hpp"
#include "ConsolaEditor.hpp"
#include "ControladorModo.hpp"
#include "SistemaRendimiento.hpp"

class SistemaUI {
private:
    bool m_MostrarEditor;

public:
    SistemaUI() : m_MostrarEditor(true) {}

    void AlternarModoEditor() { m_MostrarEditor = !m_MostrarEditor; }
    bool EstaModoEditorActivo() const { return m_MostrarEditor; }

    void DibujarPanelEditor(GestorEntidades& gestor, JerarquiaEscena& jerarquia, Inspector& inspector, const ControladorModo& controlador, SistemaRendimiento& rendimiento) {
        if (!m_MostrarEditor) return;

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, 1280, 720, 0, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glDisable(GL_DEPTH_TEST);

        // Barra Superior
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

        // Panel Jerarquía (Izquierda) - Fondo
        glColor4f(0.06f, 0.06f, 0.09f, 0.85f);
        glBegin(GL_QUADS);
            glVertex2f(10.0f, 45.0f);
            glVertex2f(280.0f, 45.0f);
            glVertex2f(280.0f, 520.0f);
            glVertex2f(10.0f, 520.0f);
        glEnd();

        // Panel Inspector (Derecha) - Fondo
        glColor4f(0.06f, 0.06f, 0.09f, 0.85f);
        glBegin(GL_QUADS);
            glVertex2f(1000.0f, 45.0f);
            glVertex2f(1270.0f, 45.0f);
            glVertex2f(1270.0f, 520.0f);
            glVertex2f(1000.0f, 520.0f);
        glEnd();

        // Panel Consola (Inferior) - Fondo
        glColor4f(0.03f, 0.03f, 0.04f, 0.90f);
        glBegin(GL_QUADS);
            glVertex2f(10.0f, 530.0f);
            glVertex2f(1270.0f, 530.0f);
            glVertex2f(1270.0f, 710.0f);
            glVertex2f(10.0f, 710.0f);
        glEnd();

        // Restaurar estado de profundidad antes de procesar lógica de módulos secundarios
        glEnable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        // Renderizado lógico de los paneles del editor conectados
        jerarquia.RenderizarArbol(gestor);
        
        // Conectamos el Inspector con su método correcto
        inspector.MostrarInspector(gestor);

        // Renderizamos nuestro sistema universal de rendimiento y monitoreo térmico
        rendimiento.RenderizarPanelRendimiento();
    }
};
