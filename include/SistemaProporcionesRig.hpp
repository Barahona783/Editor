#pragma once
#include <string>
#include <vector>
#include <unordered_map>

// ==========================================
// SUBSISTEMA DE PROPORCIONES ANATÓMICAS Y RIGGING PROCEDURAL
// Basado en el canon de unidades de cabeza (A) y jerarquía de esqueleto 3D
// ==========================================

struct ComponenteProporcionesRig {
    float UnidadCabezaA; // Módulo base A (altura de la cabeza en unidades del mundo, ej. 0.25m)
    std::unordered_map<std::string, float> LongitudesHuesos;

    ComponenteProporcionesRig(float unidadA = 0.25f) : UnidadCabezaA(unidadA) {
        // Inicialización de proporciones según esquemas anatómicos estándar (Canon de 8 cabezas):
        LongitudesHuesos["head"] = 1.0f * unidadA;
        LongitudesHuesos["neck"] = 0.5f * unidadA;
        LongitudesHuesos["chest"] = 1.5f * unidadA;       // Torso / Pecho (1.5A)
        LongitudesHuesos["pelvis"] = 1.0f * unidadA;      // Pelvis / Bacinete (1A)
        LongitudesHuesos["upper_arm"] = 1.5f * unidadA;   // Brazo superior (1.5A)
        LongitudesHuesos["forearm"] = 1.5f * unidadA;     // Antebrazo (1.5A)
        LongitudesHuesos["thigh"] = 2.0f * unidadA;       // Muslo (2A)
        LongitudesHuesos["shin"] = 2.0f * unidadA;        // Pantorrilla / Espinilla (2A)
        LongitudesHuesos["foot"] = 0.5f * unidadA;        // Altura del pie (0.5A)
    }
};

class SistemaProporcionesRig {
public:
    // Recalcula dinámicamente las longitudes de los huesos del esqueleto en función del módulo A
    static void ActualizarProporcionesEsqueleto(ComponenteProporcionesRig& rig) {
        rig.LongitudesHuesos["head"] = 1.0f * rig.UnidadCabezaA;
        rig.LongitudesHuesos["neck"] = 0.5f * rig.UnidadCabezaA;
        rig.LongitudesHuesos["chest"] = 1.5f * rig.UnidadCabezaA;
        rig.LongitudesHuesos["pelvis"] = 1.0f * rig.UnidadCabezaA;
        rig.LongitudesHuesos["upper_arm"] = 1.5f * rig.UnidadCabezaA;
        rig.LongitudesHuesos["forearm"] = 1.5f * rig.UnidadCabezaA;
        rig.LongitudesHuesos["thigh"] = 2.0f * rig.UnidadCabezaA;
        rig.LongitudesHuesos["shin"] = 2.0f * rig.UnidadCabezaA;
        rig.LongitudesHuesos["foot"] = 0.5f * rig.UnidadCabezaA;
    }

    // Ciclo de actualización ejecutado por el motor en cada fotograma
    template <typename GestorEntidadesTipo>
    static void Actualizar(GestorEntidadesTipo& gestorEntidades) {
        auto entidades = gestorEntidades.ObtenerTodasLasEntidades();
        for (auto entidad : entidades) {
            auto* comp = gestorEntidades.template ObtenerComponente<ComponenteProporcionesRig>(entidad);
            if (comp) {
                ActualizarProporcionesEsqueleto(*comp);
            }
        }
    }
};
