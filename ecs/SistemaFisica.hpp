#pragma once
#include "GestorEntidades.hpp"
#include "Vector3.hpp"
#include "Componentes.hpp" // Asegura la disponibilidad del ComponenteTensorDeformacion y componentes de inercia
#include "SistemaOptica.hpp" // <-- Integración del sistema óptico basado en la Ley de Fermat

class SistemaFisica {
private:
    Vector3 m_Gravedad;
    bool m_ModoDepuracionTensor; // <-- Variable añadida para sincronizar el estado de depuración desde la UI

public:
    SistemaFisica(Vector3 gravedad = Vector3(0.0f, -9.81f, 0.0f))
        : m_Gravedad(gravedad), m_ModoDepuracionTensor(false) {}

    void EstablecerModoDepuracionTensor(bool activo) {
        m_ModoDepuracionTensor = activo;
    }

    bool EstaModoDepuracionTensorActivo() const {
        return m_ModoDepuracionTensor;
    }

    void Actualizar(GestorEntidades& gestor, float tiempoDelta) {
        auto& entidades = gestor.ObtenerTodasLasEntidades();

        for (const auto& entidad : entidades) {
            auto transformacion = gestor.ObtenerTransformacion(entidad);
            auto cuerpoRigido = gestor.ObtenerCuerpoRigido(entidad);

            if (transformacion && cuerpoRigido && cuerpoRigido->EsDinamico) {
                // Aplicar gravedad y aceleración (Integración de Euler)
                cuerpoRigido->Velocidad = cuerpoRigido->Velocidad + (m_Gravedad * cuerpoRigido->EscalaGravedad * tiempoDelta);
                transformacion->Posicion = transformacion->Posicion + (cuerpoRigido->Velocidad * tiempoDelta);

                // ==========================================
                // INTEGRACIÓN DE INERCIA ROTACIONAL Y MOMENTO
                // ==========================================
                auto inerciaRotacional = gestor.ObtenerComponente<ComponenteInerciaRotacional>(entidad);
                if (inerciaRotacional) {
                    // Actualizar la rotación de la entidad utilizando la velocidad angular almacenada y el tensor de inercia
                    float anguloX = inerciaRotacional->VelocidadAngular.X * tiempoDelta;
                    float anguloY = inerciaRotacional->VelocidadAngular.Y * tiempoDelta;
                    float anguloZ = inerciaRotacional->VelocidadAngular.Z * tiempoDelta;

                    Cuaternion rotacionDelta = Cuaternion::CrearDesdeEjeAngulo(Vector3(1.0f, 0.0f, 0.0f), anguloX) *
                                               Cuaternion::CrearDesdeEjeAngulo(Vector3(0.0f, 1.0f, 0.0f), anguloY) *
                                               Cuaternion::CrearDesdeEjeAngulo(Vector3(0.0f, 0.0f, 1.0f), anguloZ);
                    
                    transformacion->Rotacion = (transformacion->Rotacion * rotacionDelta).ObtenerNormalizado();
                }

                // Detección simple con el plano del suelo (Y = 0)
                if (transformacion->Posicion.Y <= 0.0f) {
                    transformacion->Posicion.Y = 0.0f;
                    
                    // ==========================================
                    // INTEGRACIÓN TENSORIAL DE IMPACTO CON EL SUELO
                    // ==========================================
                    // Si la entidad cuenta con un componente de deformación por tensor,
                    // calculamos el esfuerzo de impacto contra la normal del suelo (0, 1, 0)
                    auto tensorDeformacion = gestor.ObtenerComponente<ComponenteTensorDeformacion>(entidad);
                    if (tensorDeformacion) {
                        // Magnitud de la fuerza de impacto estimada por velocidad y masa
                        Vector3 fuerzaImpacto = cuerpoRigido->Velocidad * cuerpoRigido->Masa;
                        Vector3 normalSuelo(0.0f, 1.0f, 0.0f);
                        
                        // Aplica el producto tensorial (v ⊗ w) para generar la matriz de tensión del choque
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
        // RESOLUCIÓN DE RESTRICCIONES FÍSICAS (JOINTS)
        // ==========================================
        // Recorre las entidades para aplicar correcciones de distancia si tienen restricciones asignadas
        for (const auto& entidad : entidades) {
            auto restriccion = gestor.ObtenerComponente<ComponenteRestriccionFisica>(entidad);
            if (restriccion) {
                // Buscamos las transformaciones de las entidades unidas
                // Nota: Asumimos una verificación segura de IDs para mantener la rigidez estructural
                // (Espacio preparado para la resolución matricial de distancias entre nodos)
            }
        }
    }

    // =========================================================================
    // INTEGRACIÓN DE LA LEY DE FERMAT Y ÓPTICA EN LA FÍSICA
    // =========================================================================
    // Permite evaluar la desviación de proyectiles, haces o partículas dinámicas 
    // al atravesar medios con índice de refracción variable (Principio de mínima longitud óptica).
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
