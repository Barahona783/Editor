#pragma once
#include <cmath>

struct EstadoEsfuerzos {
    float sigma_x;
    float sigma_y;
    float tau_xy;
};

struct ResultadosMohr {
    float centro;
    float radio;
    float sigma_1;
    float sigma_2;
    float tau_max;
    float anguloPrincipalGrados;
};

class CirculoMohr {
public:
    static ResultadosMohr Calcular(const EstadoEsfuerzos& estado) {
        ResultadosMohr res;
        
        // Centro del círculo en el eje normal (sigma)
        res.centro = (estado.sigma_x + estado.sigma_y) * 0.5f;
        
        // Radio del círculo (máximo esfuerzo cortante en el plano)
        float diff = (estado.sigma_x - estado.sigma_y) * 0.5f;
        res.radio = std::sqrt(diff * diff + estado.tau_xy * estado.tau_xy);
        
        // Esfuerzos principales principal mayor y menor
        res.sigma_1 = res.centro + res.radio;
        res.sigma_2 = res.centro - res.radio;
        
        // Cortante máximo
        res.tau_max = res.radio;
        
        // Ángulo principal en radianes y luego conversión a grados
        float rad = 0.5f * std::atan2(2.0f * estado.tau_xy, estado.sigma_x - estado.sigma_y);
        res.anguloPrincipalGrados = rad * (180.0f / 3.14159265f);
        
        return res;
    }
};
