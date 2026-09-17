#include <iostream>
#include <memory>
#include <vector>
#include <GL/gl.h>

// --- Cabeceras include/ ---
#include "Plataforma.hpp"
#include "ContextoGrafico.hpp"
#include "Ventana.hpp"
#include "Matriz4x4.hpp"
#include "Malla.hpp"
#include "Luz3D.hpp"

// --- Cabeceras ecs/ ---
#include "Entidad.hpp"
#include "Componentes.hpp"
#include "ComponenteJerarquia.hpp"
#include "GestorEntidades.hpp"
#include "ControladorModo.hpp"
#include "JerarquiaEscena.hpp"
#include "Inspector.hpp"
#include "ConsolaEditor.hpp"
#include "SistemaUI.hpp"
#include "SistemaRendimiento.hpp" // <--- 1. Incluimos nuestro subsistema universal
#include "SistemaSeleccion.hpp"
#include "Entrada.hpp"
#include "SerializadorEscena.hpp"

int main() {
    // Creación de la ventana usando el patrón Factory nativo del motor
    PropiedadesVentana props("Motor 3D - Modo Editor", 800, 600);
    std::unique_ptr<Ventana> ventana(Ventana::Crear(props));

    if (!ventana) {
        std::cerr << "Error: No se pudo crear la ventana de la plataforma." << std::endl;
        return -1;
    }

    ContextoGrafico* contexto = ventana->ObtenerContextoGrafico();
    if (contexto) {
        contexto->Inicializar();
    }

    glEnable(GL_DEPTH_TEST);

    GestorEntidades gestorEntidades;
    ControladorModo controladorModo;
    JerarquiaEscena jerarquiaEscena;
    Inspector inspector;
    SistemaUI sistemaUI;
    SistemaRendimiento sistemaRendimiento; // <--- 2. Instanciamos el sistema de rendimiento
    SistemaSeleccion sistemaSeleccion;     // <--- 3. Instanciamos el sistema de selección que añadimos

    Entidad objetoPrincipal = gestorEntidades.CrearEntidad("ObjetoCubo");
    gestorEntidades.AsignarTransformacion(objetoPrincipal, ComponenteTransformacion(Vector3(0.0f, 0.0f, -5.0f)));

    while (ventana->EstaEjecutandose()) {
        ventana->AlActualizar();

        if (contexto) {
            contexto->EstableserColorLimpieza(0.1f, 0.1f, 0.12f, 1.0f);
            contexto->LimpiarPantalla();
        }

        ComponenteTransformacion* trans = gestorEntidades.ObtenerTransformacion(objetoPrincipal);
        if (trans) {
            trans->Posicion.Y += 0.001f;
        }

        // 3. Pasamos el sistema de rendimiento al panel del editor
        sistemaUI.DibujarPanelEditor(gestorEntidades, jerarquiaEscena, inspector, controladorModo, sistemaRendimiento);

        if (contexto) {
            contexto->IntercambiarBúferes();
        }
    }

    // Guardado automático de la escena al cerrar el motor de forma segura
    SerializadorEscena::GuardarEscena(gestorEntidades, "escena_guardada.txt");

    return 0;
}
