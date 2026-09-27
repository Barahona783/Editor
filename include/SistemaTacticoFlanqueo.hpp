#pragma once
#include <vector>
#include <string>

// ==========================================
// SUBSISTEMA TÁCTICO DE FLANQUEO Y COBERTURAS
// Basado en mapas de bloqueo (blockout), puntos de cobertura y rutas de flanqueo
// ==========================================

struct ComponenteTacticoFlanqueo {
    bool EsPuntoDeCobertura;
    bool EsRutaFlanqueo;
    float PrioridadTactica;
    std::string TipoEstructura; // "Estanteria", "Columna", "Mostrador", "Vehiculo"

    ComponenteTacticoFlanqueo(bool cobertura = true, bool flanqueo = false, float prioridad = 1.0f, const std::string& tipo = "Cobertura")
        : EsPuntoDeCobertura(cobertura), EsRutaFlanqueo(flanqueo), PrioridadTactica(prioridad), TipoEstructura(tipo) {}
};

class SistemaTacticoFlanqueo {
public:
    // Evalúa la validez de los puntos de cobertura y calcula rutas de flanqueo para la IA
    static void EvaluarRutasTacticas(ComponenteTacticoFlanqueo& tactica) {
        if (tactica.EsRutaFlanqueo) {
            tactica.PrioridadTactica = 2.5f; // Las rutas de flanqueo tienen alta prioridad dinámica
        }
    }

    // Ciclo de actualización global del sistema táctico en el motor
    template <typename GestorEntidadesTipo>
    static void Actualizar(GestorEntidadesTipo& gestorEntidades) {
        auto entidades = gestorEntidades.ObtenerTodasLasEntidades();
        for (auto entidad : entidades) {
            auto* comp = gestorEntidades.template ObtenerComponente<ComponenteTacticoFlanqueo>(entidad);
            if (comp) {
                EvaluarRutasTacticas(*comp);
            }
        }
    }
};
