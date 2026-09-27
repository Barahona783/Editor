#pragma once
#include "GestorEntidades.hpp"
#include "Vector3.hpp"
#include "Componentes.hpp"
#include "SistemaOptica.hpp"

// ==========================================
// SISTEMA DE FÍSICA Y SIMULACIÓN DINÁMICA DEL MOTOR
// ==========================================
class SistemaFisica {
private:
    Vector3 m_Gravedad;
    bool m_ModoDepuracionTensor; // Sincroniza el estado de depuración tensorial desde la UI

public:
    SistemaFisica(Vector3 gravedad = Vector3(0.0f, -9.81f, 0.0f))
        : m_Gravedad(gravedad), m_ModoDepuracionTensor(false) {}

    void EstablecerModoDepuracionTensor(bool activo) {
        m_ModoDepuracionTensor = activo;
    }

    bool EstaModoDepuracionTensorActivo() const {
        return m_ModoDepuracionTensor;
    }

    // Actualiza la simulación física de todas las entidades en cada frame
    void Actualizar(GestorEntidades& gestor, float tiempoDelta) {
        const auto& entidades = gestor.ObtenerTodasLasEntidades();

        for (const auto& entidad : entidades) {
            auto transformacion = gestor.ObtenerTransformacion(entidad);
            auto cuerpoRigido = gestor.ObtenerCuerpoRigido(entidad);

            if (transformacion && cuerpoRigido && cuerpoRigido->EsDinamico) {
                // 1. Aplicar gravedad y aceleración (Integración de Euler)
                cuerpoRigido->Velocidad = cuerpoRigido->Velocidad + (m_Gravedad * cuerpoRigido->EscalaGravedad * tiempoDelta);
                transformacion->Posicion = transformacion->Posicion + (cuerpoRigido->Velocidad * tiempoDelta);

                // ==========================================
                // 2. INTEGRACIÓN DE INERCIA ROTACIONAL Y MOMENTO
                // ==========================================
                auto inerciaRotacional = gestor.ObtenerComponente<ComponenteInerciaRotacional>(entidad);
                if (inerciaRotacional) {
                    float anguloX = inerciaRotacional->VelocidadAngular.X * tiempoDelta;
                    float anguloY = inerciaRotacional->VelocidadAngular.Y * tiempoDelta;
                    float anguloZ = inerciaRotacional->VelocidadAngular.Z * tiempoDelta;

                    Cuaternion rotacionDelta = Cuaternion::CrearDesdeEjeAngulo(Vector3(1.0f, 0.0f, 0.0f), anguloX) *
                                               Cuaternion::CrearDesdeEjeAngulo(Vector3(0.0f, 1.0f, 0.0f), anguloY) *
                                               Cuaternion::CrearDesdeEjeAngulo(Vector3(0.0f, 0.0f, 1.0f), anguloZ);
                    
                    transformacion->Rotacion = (transformacion->Rotacion * rotacionDelta).ObtenerNormalizado();
                }

                // 3. Detección de colisión con el plano base del suelo (Y = 0)
                if (transformacion->Posicion.Y <= 0.0f) {
                    transformacion->Posicion.Y = 0.0f;
                    
                    // ==========================================
                    // 4. INTEGRACIÓN TENSORIAL DE IMPACTO CON EL SUELO
                    // ==========================================
                    auto tensorDeformacion = gestor.ObtenerComponente<ComponenteTensorDeformacion>(entidad);
                    if (tensorDeformacion) {
                        Vector3 fuerzaImpacto = cuerpoRigido->Velocidad * cuerpoRigido->Masa;
                        Vector3 normalSuelo(0.0f, 1.0f, 0.0f);
                        
                        // Genera la matriz de tensión del choque mediante el producto tensorial
                        tensorDeformacion->AplicarImpactoTensorial(fuerzaImpacto, normalSuelo);
                    }

                    cuerpoRigido->Velocidad.Y = 0.0f;
                    cuerpoRigido->EnElSuelo = true;
                } else {
                    cuerpoRigido->EnElSuelo = false;
                }
            }
        }

        // ==========================================
        // 5. RESOLUCIÓN DE RESTRICCIONES FÍSICAS (JOINTS)
        // ==========================================
        for (const auto& entidad : entidades) {
            auto restriccion = gestor.ObtenerComponente<ComponenteRestriccionFisica>(entidad);
            if (restriccion) {
                // Resolución matricial de distancias entre nodos conectados
            }
        }
    }

    // =========================================================================
    // INTEGRACIÓN DE LA LEY DE FERMAT Y ÓPTICA EN LA FÍSICA DINÁMICA
    // =========================================================================
    void AplicarRefraccionOpticaFisica(Vector3& posicion, Vector3& velocidad, const SistemaOptica::MedioOptico& medio) {
        float factorDistorsion = SistemaOptica::ObtenerFactorDistorsionEspacial(posicion, medio);
        if (factorDistorsion != 0.0f && velocidad.Magnitud() > 0.0f) {
            Vector3 normalSimulada = (medio.PosicionCentro - posicion).Normalizado();
            Vector3 direccionNormalizada = velocidad.Normalizado();
            
            Vector3 nuevaDireccion = SistemaOptica::DesviacionRayon(
                direccionNormalizada, 
                normalSimulada, 
                1.0f, 
                medio.IndiceRefraction
            );
            
            float velocidadMagnitud = velocidad.Magnitud();
            velocidad = nuevaDireccion * velocidadMagnitud;
        }
    }

    void EstablecerGravedad(const Vector3& gravedad) {
        m_Gravedad = gravedad;
    }
};
