#pragma once
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
#include "RedNeuronal.hpp"          // Subsistema de red neuronal
#include "SistemaMundoAbierto.hpp"   // Subsistema de streaming para mundo abierto
#include "SistemaOptica.hpp"        // Subsistema de óptica y Ley de Fermat (OPL)
#include "SistemaAnimacion.hpp"     // Subsistema de animación por keyframes y reducción

// --- Nuevos Subsistemas de Conciencia Espacial, Navegación y Aerodinámica ---
#include "SistemaNavegacion.hpp"
#include "SistemaEdificios.hpp"
#include "SistemaPercepcion.hpp"
#include "SistemaAerodinamica.hpp"   // Subsistema universal de aerodinámica (Navier-Stokes simplificado)

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
#include "SistemaRendimiento.hpp"
#include "SistemaSeleccion.hpp"
#include "SistemaFisica.hpp"        // Integración del sistema físico y tensorial
#include "Entrada.hpp"
#include "SerializadorEscena.hpp"
#include "GestorPlugins.hpp"
#include "Comportamiento.hpp"       // Nuestro sistema de scripts nativos
#include "GizmosEditor.hpp"         // Gizmos de transformación 3D

// --- Nuevos Sistemas Nativos de Alto Rendimiento ---
#include "BusEventos.hpp"
#include "SistemaComandos.hpp"
#include "SistemaParticulas.hpp"

int main() {
    PropiedadesVentana props("Motor 3D - Modo Editor (Mundo Abierto)", 800, 600);
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
    SistemaRendimiento sistemaRendimiento;
    SistemaSeleccion sistemaSeleccion;
    GizmosEditor gizmosEditor;                  
    GestorPlugins gestorPlugins;
    SistemaFisica sistemaFisica;                 
    SistemaMundoAbierto sistemaMundoAbierto(80.0f); // Streaming de mundo abierto con radio de 80 unidades

    // Instanciación de los nuevos sistemas espaciales, de edificios, percepción y aerodinámica
    SistemaNavegacion sistemaNavegacion;
    SistemaEdificios sistemaEdificios;
    SistemaPercepcion sistemaPercepcion;
    SistemaAerodinamica sistemaAerodinamica(Vector3(1.5f, 0.0f, 0.5f)); // Viento global ambiental inicial

    // Definición de un medio óptico global de prueba en el mundo (ej. campo de distorsión o zona de refracción)
    SistemaOptica::MedioOptico medioOpticoGlobal(1.33f, Vector3(0.0f, 2.0f, -5.0f), 15.0f);

    // Instanciamos nuestros nuevos sistemas nativos del núcleo
    SistemaComandos& sistemaComandos = SistemaComandos::ObtenerInstancia();
    SistemaParticulas sistemaParticulas(300); // Emisor con capacidad para 300 partículas

    // --- Integración del Serializador: Intentamos cargar la escena guardada previamente ---
    if (!SerializadorEscena::CargarEscena(gestorEntidades, "escena_guardada.txt")) {
        Entidad objetoPrincipal = gestorEntidades.CrearEntidad("ObjetoCubo");
        gestorEntidades.AsignarTransformacion(objetoPrincipal, ComponenteTransformacion(Vector3(0.0f, 0.0f, -5.0f)));
        
        // Asignamos capacidades utilizando los métodos base confirmados del gestor
        gestorEntidades.AsignarCuerpoRigido(objetoPrincipal, ComponenteCuerpoRigido(2.0f, true, 1.0f));
        gestorEntidades.AsignarTensorDeformacion(objetoPrincipal, ComponenteTensorDeformacion(150.0f));
    }

    // Ejemplo de prueba: Ejecutar un comando nativo al iniciar el motor
    sistemaComandos.EjecutarLinea("spawn CuboGeneradoPorConsola", gestorEntidades);

    while (ventana->EstaEjecutandose()) {
        ventana->AlActualizar();

        if (contexto) {
            contexto->EstableserColorLimpieza(0.1f, 0.1f, 0.12f, 1.0f);
            contexto->LimpiarPantalla();
        }

        // --- Actualización de Sistemas de Lógica, Física, IA y Mundo Abierto ---
        
        // Sincronizamos el estado del botón de depuración tensorial de la UI con el sistema físico
        sistemaFisica.EstablecerModoDepuracionTensor(sistemaUI.EstaDepuracionTensorActiva());

        // Obtener entidades activas para referencias de posición
        auto entidadesActivas = gestorEntidades.ObtenerTodasLasEntidades();
        float jugadorX = 0.0f;
        float jugadorZ = 0.0f;

        if (!entidadesActivas.empty()) {
            ComponenteTransformacion* trans = gestorEntidades.ObtenerTransformacion(entidadesActivas[0]);
            if (trans) {
                jugadorX = trans->Posicion.X;
                jugadorZ = trans->Posicion.Z;

                // Aplicar refracción óptica basada en la Ley de Fermat sobre la velocidad/dinámica del objeto si entra al medio
                auto cuerpoRigidoEntidad = gestorEntidades.ObtenerCuerpoRigido(entidadesActivas[0]);
                if (cuerpoRigidoEntidad) {
                    sistemaFisica.AplicarRefraccionOpticaFisica(trans->Posicion, cuerpoRigidoEntidad->Velocidad, medioOpticoGlobal);
                }

                // Emitir partículas de prueba desde la posición de la primera entidad en cada frame
                sistemaParticulas.EmitirParticula(
                    Vector3(trans->Posicion.X, trans->Posicion.Y, trans->Posicion.Z),
                    Vector3(0.0f, 0.5f, 0.0f),
                    1.5f,
                    4.0f
                );
            }
        }

        // Procesar carga y streaming de chunks/sectores para el mundo abierto basado en la posición del jugador
        sistemaMundoAbierto.ActualizarStreaming(gestorEntidades, jugadorX, jugadorZ);

        // --- Actualización de los nuevos subsistemas de Conciencia Espacial, Navegación, Aerodinámica y Animación ---
        sistemaNavegacion.Actualizar(gestorEntidades, 0.016f);
        sistemaEdificios.ActualizarYVerificarProximidadAgentes(gestorEntidades);
        sistemaPercepcion.Actualizar(gestorEntidades);
        sistemaAerodinamica.Actualizar(gestorEntidades, 0.016f); // Procesa fuerzas de arrastre y sustentación universal
        SistemaAnimacion::Actualizar(gestorEntidades, 0.016f);  // Procesa la interpolación de fotogramas clave por keyframes

        sistemaFisica.Actualizar(gestorEntidades, 0.016f); // Procesa gravedad, movimientos y cálculo tensorial de impactos
        SistemaScripts::Actualizar(gestorEntidades, 0.016f);

        // ==========================================
        // CICLO DE INFERENCIA DE REDES NEURONALES (IA)
        // ==========================================
        for (const auto& entidad : entidadesActivas) {
            ComponenteRedNeuronal* redNeuronal = gestorEntidades.ObtenerComponente<ComponenteRedNeuronal>(entidad);
            ComponenteTransformacion* transformacion = gestorEntidades.ObtenerTransformacion(entidad);
            ComponenteSector* sector = gestorEntidades.ObtenerComponente<ComponenteSector>(entidad);

            // Solo procesamos la red neuronal si el sector está activo en memoria (optimización de mundo abierto)
            bool sectorActivo = sector ? sector->ActivoEnMemoria : true;

            if (redNeuronal && redNeuronal->Activo && transformacion && sectorActivo) {
                std::vector<float> entradasSensoriales = {
                    transformacion->Posicion.X,
                    transformacion->Posicion.Y,
                    transformacion->Posicion.Z
                };

                std::vector<float> decisionNeuronal = redNeuronal->Cerebro.Predecir(entradasSensoriales);
            }
        }

        // Actualizar física/vida de las partículas
        sistemaParticulas.Actualizar(0.016f);
        sistemaParticulas.Renderizar();

        // Actualizar plugins externos
        gestorPlugins.ActualizarPlugins(gestorEntidades, 0.016f);

        // --- Dibujar interfaz y paneles del editor ---
        sistemaUI.DibujarPanelEditor(gestorEntidades, jerarquiaEscena, inspector, controladorModo, sistemaRendimiento, sistemaSeleccion);

        // ==========================================
        // RENDERIZADO DE GIZMOS 3D SOBRE LA ENTIDAD SELECCIONADA
        // ==========================================
        Entidad entidadSeleccionada = sistemaSeleccion.ObtenerEntidadSeleccionada();
        if (entidadSeleccionada.ObtenerID() != 0) {
            ComponenteTransformacion* transSeleccionada = gestorEntidades.ObtenerTransformacion(entidadSeleccionada);
            if (transSeleccionada) {
                gizmosEditor.RenderizarGizmo3D(*transSeleccionada);
                gizmosEditor.RenderizarDepuracionEspacial(*transSeleccionada); // Dibuja guías espaciales y de ruta
            }
        }

        if (contexto) {
            contexto->IntercambiarBúferes();
        }
    }

    gestorPlugins.DescargarTodosLosPlugins();
    SerializadorEscena::GuardarEscena(gestorEntidades, "escena_guardada.txt");

    return 0;
}
