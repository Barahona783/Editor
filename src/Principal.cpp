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

// --- Nuevos Subsistemas de Conciencia Espacial, Navegación, Aerodinámica e Iluminación ---
#include "SistemaNavegacion.hpp"
#include "SistemaEdificios.hpp"
#include "SistemaPercepcion.hpp"
#include "SistemaAerodinamica.hpp"          // Subsistema universal de aerodinámica
#include "SistemaModelado3D.hpp"            // Subsistema de modelado 3D y mallas editables
#include "SistemaIluminacionVolumetrica.hpp" // Subsistema de luces volumétricas y conos de luz

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
    PropiedadesVentana props("Motor 3D - Modo Editor (Volumétrica & Modelado)", 800, 600);
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

    // Instanciación de los subsistemas del motor
    SistemaNavegacion sistemaNavegacion;
    SistemaEdificios sistemaEdificios;
    SistemaPercepcion sistemaPercepcion;
    SistemaAerodinamica sistemaAerodinamica(Vector3(1.5f, 0.0f, 0.5f)); // Viento global ambiental inicial
    SistemaAnimacion sistemaAnimacion; 
    SistemaIluminacionVolumetrica sistemaIluminacionVolumetrica; // Subsistema de luces volumétricas y haces de luz

    // Definición de un medio óptico global de prueba en el mundo
    SistemaOptica::MedioOptico medioOpticoGlobal(1.33f, Vector3(0.0f, 2.0f, -5.0f), 15.0f);

    // Instanciamos sistemas nativos del núcleo
    SistemaComandos& sistemaComandos = SistemaComandos::ObtenerInstancia();
    SistemaParticulas sistemaParticulas(300); // Emisor con capacidad para 300 partículas

    // --- Integración del Serializador: Intentamos cargar la escena o inicializar con Elementos y Luces Volumétricas ---
    if (!SerializadorEscena::CargarEscena(gestorEntidades, "escena_guardada.txt")) {
        // 1. Crear entidad con geometría procedural generada por el Sistema de Modelado 3D
        Entidad objetoCuboModelado = gestorEntidades.CrearEntidad("CuboModeladoProcedural");
        gestorEntidades.AsignarTransformacion(objetoCuboModelado, ComponenteTransformacion(Vector3(0.0f, 1.0f, -5.0f)));
        gestorEntidades.AsignarCuerpoRigido(objetoCuboModelado, ComponenteCuerpoRigido(2.0f, true, 1.0f));
        gestorEntidades.AsignarTensorDeformacion(objetoCuboModelado, ComponenteTensorDeformacion(150.0f));

        MallaEditable mallaEditable = SistemaModelado3D::GenerarCubo(2.0f, 2.0f, 2.0f);

        // 2. Crear entidad de Terreno para el mundo abierto
        Entidad entidadTerreno = gestorEntidades.CrearEntidad("TerrenoMundoAbierto");
        gestorEntidades.AsignarTransformacion(entidadTerreno, ComponenteTransformacion(Vector3(0.0f, -3.0f, 0.0f)));
        
        ComponenteTerreno terrenoBase(64, 64, 2.0f, 30.0f);
        terrenoBase.ModificarAltura(32, 32, 10.0f);

        // 3. Crear Entidad con Foco de Luz Volumétrica (Efecto haz de luz realista tipo Blender)
        Entidad entidadLuzVol = gestorEntidades.CrearEntidad("FocoLuzVolumetrica");
        gestorEntidades.AsignarTransformacion(entidadLuzVol, ComponenteTransformacion(Vector3(0.0f, 5.0f, -3.0f)));
        // Asignamos el componente de luz volumétrica con tono cálido y alta intensidad
        gestorEntidades.AsignarComponente<ComponenteLuzVolumetrica>(entidadLuzVol, ComponenteLuzVolumetrica("LuzEscenario", Vector3(1.0f, 0.85f, 0.6f), 4.0f, 20.0f, 45.0f, 0.6f));
    }

    sistemaComandos.EjecutarLinea("spawn CuboGeneradoPorConsola", gestorEntidades);

    while (ventana->EstaEjecutandose()) {
        ventana->AlActualizar();

        if (contexto) {
            contexto->EstableserColorLimpieza(0.1f, 0.1f, 0.12f, 1.0f);
            contexto->LimpiarPantalla();
        }

        // Sincronizamos el estado del botón de depuración tensorial de la UI con el sistema físico
        sistemaFisica.EstablecerModoDepuracionTensor(sistemaUI.EstaDepuracionTensorActiva());

        auto entidadesActivas = gestorEntidades.ObtenerTodasLasEntidades();
        float jugadorX = 0.0f;
        float jugadorZ = 0.0f;

        if (!entidadesActivas.empty()) {
            ComponenteTransformacion* trans = gestorEntidades.ObtenerTransformacion(entidadesActivas[0]);
            if (trans) {
                jugadorX = trans->Posicion.X;
                jugadorZ = trans->Posicion.Z;

                auto cuerpoRigidoEntidad = gestorEntidades.ObtenerCuerpoRigido(entidadesActivas[0]);
                if (cuerpoRigidoEntidad) {
                    sistemaFisica.AplicarRefraccionOpticaFisica(trans->Posicion, cuerpoRigidoEntidad->Velocidad, medioOpticoGlobal);
                }

                sistemaParticulas.EmitirParticula(
                    Vector3(trans->Posicion.X, trans->Posicion.Y, trans->Posicion.Z),
                    Vector3(0.0f, 0.5f, 0.0f),
                    1.5f,
                    4.0f
                );
            }
        }

        // Streaming y actualización de subsistemas
        sistemaMundoAbierto.ActualizarStreaming(gestorEntidades, jugadorX, jugadorZ);
        sistemaNavegacion.Actualizar(gestorEntidades, 0.016f);
        sistemaEdificios.ActualizarYVerificarProximidadAgentes(gestorEntidades);
        sistemaPercepcion.Actualizar(gestorEntidades);
        sistemaAerodinamica.Actualizar(gestorEntidades, 0.016f);
        sistemaAnimacion.Actualizar(gestorEntidades, 0.016f);
        
        // Actualizamos el subsistema de iluminación volumétrica para recalcular haces de luz
        sistemaIluminacionVolumetrica.Actualizar(gestorEntidades);

        sistemaFisica.Actualizar(gestorEntidades, 0.016f);
        SistemaScripts::Actualizar(gestorEntidades, 0.016f);

        // Ciclo de Inferencia de Redes Neuronales (IA)
        for (const auto& entidad : entidadesActivas) {
            ComponenteRedNeuronal* redNeuronal = gestorEntidades.ObtenerComponente<ComponenteRedNeuronal>(entidad);
            ComponenteTransformacion* transformacion = gestorEntidades.ObtenerTransformacion(entidad);
            ComponenteSector* sector = gestorEntidades.ObtenerComponente<ComponenteSector>(entidad);

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

        sistemaParticulas.Actualizar(0.016f);
        sistemaParticulas.Renderizar();
        gestorPlugins.ActualizarPlugins(gestorEntidades, 0.016f);

        // Interfaz y Paneles del Editor
        sistemaUI.DibujarPanelEditor(gestorEntidades, jerarquiaEscena, inspector, controladorModo, sistemaRendimiento, sistemaSeleccion);

        Entidad entidadSeleccionada = sistemaSeleccion.ObtenerEntidadSeleccionada();
        if (entidadSeleccionada.ObtenerID() != 0) {
            ComponenteTransformacion* transSeleccionada = gestorEntidades.ObtenerTransformacion(entidadSeleccionada);
            if (transSeleccionada) {
                gizmosEditor.RenderizarGizmo3D(*transSeleccionada);
                gizmosEditor.RenderizarDepuracionEspacial(*transSeleccionada);
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
