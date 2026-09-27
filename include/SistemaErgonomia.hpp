#pragma once
#include <cmath>
#include <algorithm>
#include <string>

// ==========================================
// SUBSISTEMA DE ERGONOMÍA Y LÍMITES BIOMECÁNICOS
// Basado en rangos angulares de confort humano (Visión, Cabeza y Brazos)
// ==========================================

struct ComponenteErgonomia {
    // Ángulos actuales de la entidad en grados
    float AnguloCabeza;    // Rango típico: [-60° a +60°] máx, [0° a 45°] confort
    float AnguloBrazoIzq;  // Rango de rotación de hombro / brazo
    float AnguloBrazoDer;
    float AnguloVision;    // Campo de visión horizontal

    // Bandera de estado biomecánico
    bool EnZonaConfort;
    bool EnRangoMaximo;

    ComponenteErgonomia(float cabeza = 0.0f, float brazoIzq = 0.0f, float brazoDer = 0.0f, float vision = 0.0f)
        : AnguloCabeza(cabeza), AnguloBrazoIzq(brazoIzq), AnguloBrazoDer(brazoDer), AnguloVision(vision),
          EnZonaConfort(true), EnRangoMaximo(true) {}
};

class SistemaErgonomia {
public:
    // Evalúa si la rotación de la cabeza respeta los límites ergonómicos de la biomecánica humana
    static void EvaluarCabeza(ComponenteErgonomia& ergonomia) {
        // Normalizar ángulo entre -180 y 180
        while (ergonomia.AnguloCabeza > 180.0f) ergonomia.AnguloCabeza -= 360.0f;
        while (ergonomia.AnguloCabeza < -180.0f) ergonomia.AnguloCabeza += 360.0f;

        // Según el diagrama ergonómico superior:
        // - 0°: Posición cómoda frontal
        // - 45°: Posición de giro relajado/cómodo
        // - 60°: Posición máxima de rotación de cabeza sin torsión cervical forzada
        float absAngulo = std::abs(ergonomia.AnguloCabeza);

        if (absAngulo <= 45.0f) {
            ergonomia.EnZonaConfort = true;
            ergonomia.EnRangoMaximo = true;
        } else if (absAngulo <= 60.0f) {
            ergonomia.EnZonaConfort = false; // Requiere ligero esfuerzo
            ergonomia.EnRangoMaximo = true;
        } else {
            ergonomia.EnZonaConfort = false;
            ergonomia.EnRangoMaximo = false; // Fuera de rango biomecánico seguro
            // Clamping estricto para evitar posiciones anómalas
            ergonomia.AnguloCabeza = (ergonomia.AnguloCabeza > 0.0f) ? 60.0f : -60.0f;
        }
    }

    // Evalúa la postura y rotación de los brazos/hombros según los diagramas de alcance
    static void EvaluarBrazos(ComponenteErgonomia& ergonomia) {
        // El diagrama indica que los brazos tienen un ángulo óptimo frontal de +/- 15° a 30° 
        // y una rotación total combinada de hasta ~63° (hombres) / ~67.5° (mujeres)
        float absIzq = std::abs(ergonomia.AnguloBrazoIzq);
        float absDer = std::abs(ergonomia.AnguloBrazoDer);

        const float LIMITE_MAXIMO_BRAZO = 67.5f;

        if (absIzq > LIMITE_MAXIMO_BRAZO) {
            ergonomia.AnguloBrazoIzq = (ergonomia.AnguloBrazoIzq > 0.0f) ? LIMITE_MAXIMO_BRAZO : -LIMITE_MAXIMO_BRAZO;
        }
        if (absDer > LIMITE_MAXIMO_BRAZO) {
            ergonomia.AnguloBrazoDer = (ergonomia.AnguloBrazoDer > 0.0f) ? LIMITE_MAXIMO_BRAZO : -LIMITE_MAXIMO_BRAZO;
        }
    }

    // Actualización global del sistema sobre las entidades del motor
    template <typename GestorEntidadesTipo>
    static void Actualizar(GestorEntidadesTipo& gestorEntidades) {
        auto entidades = gestorEntidades.ObtenerTodasLasEntidades();
        for (auto entidad : entidades) {
            ComponenteErgonomia* comp = gestorEntidades.template ObtenerComponente<ComponenteErgonomia>(entidad);
            if (comp) {
                EvaluarCabeza(*comp);
                EvaluarBrazos(*comp);
            }
        }
    }
};
