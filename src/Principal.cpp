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
#include "SistemaMundoAbierto.hpp"   // <-- Subsistema de streaming para mundo abierto

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
#include "Comportamiento.hpp"       // Nuestro nuevo sistema de scripts nativos

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
    GestorPlugins gestorPlugins;
    SistemaFisica sistemaFisica;                 // Instanciación del sistema de física y deformación por tensores
    SistemaMundoAbierto sistemaMundoAbierto(80.0f); // Streaming de mundo abierto con radio de corte de 80 unidades

    // Instanciamos nuestros nuevos sistemas nativos del núcleo
    SistemaComandos& sistemaComandos = SistemaComandos::ObtenerInstancia();
    SistemaParticulas sistemaParticulas(300); // Emisor con capacidad para 300 partículas

    // --- Integración del Serializador: Intentamos cargar la escena guardada previamente ---
    if (!SerializadorEscena::CargarEscena(gestorEntidades, "escena_guardada.txt")) {
        Entidad objetoPrincipal = gestorEntidades.CrearEntidad("ObjetoCubo");
        gestorEntidades.AsignarTransformacion(objetoPrincipal, ComponenteTransformacion(Vector3(0.0f, 0.0f, -5.0f)));
        
        // Asignamos capacidades de cuerpo rígido, tensor de deformación, red neuronal y sector de mundo abierto
        gestorEntidades.AsignarCuerpoRigido(objetoPrincipal, ComponenteCuerpoRigido(2.0f, true, 1.0f));
        gestorEntidades.AsignarTensorDeformacion(objetoPrincipal, ComponenteTensorDeformacion(150.0f));
        gestorEntidades.AsignarComponente<ComponenteRedNeuronal>(objetoPrincipal, ComponenteRedNeuronal());
        gestorEntidades.AsignarComponente<ComponenteSector>(objetoPrincipal, ComponenteSector(0, 0));
    }

    // Ejemplo de prueba: Ejecutar un comando nativo al iniciar el motor
    sistemaComandos.EjecutarLinea("spawn CuboGeneradoPorConsola", gestorEntidades);

    while (ventana->EstaEjecutandose()) {
        ventana->AlActualizar();

        if (contexto) {
            contexto->EstableserColorLimpieza(0.1f, 0.1f, 0.12f, 1.0f);
            contexto->LimpiarPantalla();
        }

        // --- Integración del Sistema de Selección por Raycast mediante Mouse ---
        if (Entrada::EstaPresionadoBotonRaton(0)) {
            float mouseX = Entrada::ObtenerPosicionRatonX();
            float mouseY = Entrada::ObtenerPosicionRatonY();

            float rayoOrigenX = 0.0f;
            float rayoOrigenY = 0.0f;
            float rayoOrigenZ = 0.0f;
            float rayoDirX = (mouseX - 640.0f) / 400.0f;
            float rayoDirY = -(mouseY - 360.0f) / 400.0f;
            float rayoDirZ = -1.0f;

            Entidad entidadImpactada = SistemaSeleccion::SeleccionarEntidadPorRaycast(
                gestorEntidades, rayoOrigenX, rayoOrigenY, rayoOrigenZ, rayoDirX, rayoDirY, rayoDirZ
            );

            if (entidadImpactada.ObtenerID() != 0) {
                sistemaSeleccion.SeleccionarEntidad(entidadImpactada);
                std::cout << "[Principal] Entidad seleccionada mediante Raycast ID: " << entidadImpactada.ObtenerID() << std::endl;
            }
        }

        // Obtener entidades activas para referencias de posición
        auto entidadesActivas = gestorEntidades.ObtenerTodasLasEntidades();
        float jugadorX = 0.0f;
        float jugadorZ = 0.0f;

        if (!entidadesActivas.empty()) {
            ComponenteTransformacion* trans = gestorEntidades.ObtenerTransformacion(entidadesActivas[0]);
            if (trans) {
                jugadorX = trans->Posicion.X;
                jugadorZ = trans->Posicion.Z;

                // Emitir partículas de prueba desde la posición de la primera entidad en cada frame
                sistemaParticulas.EmitirParticula(
                    Vector3(trans->Posicion.X, trans->Posicion.Y, trans->Posicion.Z),
                    Vector3(0.0f, 0.5f, 0.0f),
                    1.5f,
                    4.0f
                );
            }
        }

        // --- Actualización de Sistemas de Lógica, Física, IA y Mundo Abierto ---
        
        // Sincronizamos el estado del botón de depuración tensorial de la UI con el sistema físico
        sistemaFisica.EstablecerModoDepuracionTensor(sistemaUI.EstaDepuracionTensorActiva());

        // Procesar carga y streaming de chunks/sectores para el mundo abierto basado en la posición del jugador
        sistemaMundoAbierto.ActualizarStreaming(gestorEntidades, jugadorX, jugadorZ);

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

        // --- Dibujar interfaz y paneles del editor (Pasando el sistema de selección) ---
        sistemaUI.DibujarPanelEditor(gestorEntidades, jerarquiaEscena, inspector, controladorModo, sistemaRendimiento, sistemaSeleccion);

        if (contexto) {
            contexto->IntercambiarBúferes();
        }
    }

    gestorPlugins.DescargarTodosLosPlugins();
    SerializadorEscena::GuardarEscena(gestorEntidades, "escena_guardada.txt");

    return 0;
}
