#pragma once

class ContextoGrafico {
public:
    virtual ~ContextoGrafico() = default;

    virtual bool Inicializar() = 0;
    virtual void IntercambiarBúferes() = 0;
    virtual void EstableserColorLimpieza(float rojo, float verde, float azul, float alfa) = 0;
    virtual void LimpiarPantalla() = 0;
};

