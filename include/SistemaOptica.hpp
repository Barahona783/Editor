#pragma once
#include "Vector3.hpp"
#include <cmath>
#include <algorithm>

// ==========================================
// SISTEMA DE ÓPTICA Y LEY DE FERMAT (OPL)
// ==========================================
// Implementa el principio de mínima longitud óptica (Fermat's Law: delta int(n ds) = 0)
// Utilizado tanto para la física de proyectiles/rayos en medios variables como para 
// efectos visuales de refracción y distorsión en el mundo abierto.
class SistemaOptica {
public:
    // Estructura para representar un medio con un índice de refracción (n) específico
    struct MedioOptico {
        float IndiceRefraction; // n (ej. Vacío = 1.0, Agua = 1.33, Vidrio = 1.5, Aire denso/Calor = 1.0003)
        Vector3 PosicionCentro;
        float RadioInfluencia;

        MedioOptico(float n = 1.0f, Vector3 centro = Vector3(0,0,0), float radio = 10.0f)
            : IndiceRefraction(n), PosicionCentro(centro), RadioInfluencia(radio) {}
    };

    // 1. Cálculo de la Longitud Óptica (OPL) aproximada a lo largo de un segmento de trayectoria
    // OPL = n * distancia
    static float CalcularLongitudOptica(const Vector3& inicio, const Vector3& fin, float indiceRefractor) {
        float distancia = (fin - inicio).Magnitud();
        return indiceRefractor * distancia;
    }

    // 2. Simulación física de desviación de trayectoria (Raycasting / Proyectiles o Láseres)
    // Aplica la Ley de Snell / Fermat para desviar un vector de dirección al cruzar un gradiente de índice
    static Vector3 CalcularDesviacionRayon(const Vector3& direccionIncidente, const Vector3& normalSuperficie, float n1, float n2) {
        float r = n1 / n2;
        float cosi = -Vector3::ProductoPunto(normalSuperficie, direccionIncidente);
        float sin2t = r * r * (1.0f - cosi * cosi);
        
        // Comprobación de reflexión interna total
        if (sin2t > 1.0f) {
            // Reflexión total: reflejar el vector
            return direccionIncidente + normalSuperficie * (2.0f * cosi);
        }
        
        float cost = std::sqrt(1.0f - sin2t);
        return direccionIncidente * r + normalSuperficie * (r * cosi - cost);
    }

    // 3. Cálculo de factor de distorsión visual para el entorno (Mundo Abierto / Shaders)
    // Útil para simular espejismos por calor o refracción bajo el agua en coordenadas de pantalla o mundo
    static float ObtenerFactorDistorsionEspacial(const Vector3& posicionMundo, const MedioOptico& medio) {
        float distanciaAlCentro = (posicionMundo - medio.PosicionCentro).Magnitud();
        if (distanciaAlCentro > medio.RadioInfluencia) {
            return 0.0f; // Fuera del área de influencia óptica
        }
        
        // Factor de atenuación basado en la proximidad al centro del medio y su índice de refracción
        float factorAtenuacion = 1.0f - (distanciaAlCentro / medio.RadioInfluencia);
        return (medio.IndiceRefraction - 1.0f) * factorAtenuacion;
    }
};
