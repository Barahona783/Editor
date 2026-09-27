#pragma once
#include "Entidad.hpp"
#include "Componentes.hpp"
#include "GestorEntidades.hpp"
#include "Entrada.hpp"
#include <iostream>

// Alias de compatibilidad alineado con IdentificadorEntidad del núcleo ECS
using EntidadID = IdentificadorEntidad;

class SistemaControlCamara {
private:
    EntidadID m_CamaraActivaID;

public:
    SistemaControlCamara() : m_CamaraActivaID(0) {}

    // Establece una entidad específica como la cámara activa de la escena
    void EstablecerCamaraActiva(EntidadID id) {
        m_CamaraActivaID = id;
        std::cout << "[SistemaControlCamara] Cámara activa establecida ID: " << id << "\n";
    }

    EntidadID ObtenerCamaraActivaID() const {
        return m_CamaraActivaID;
    }

    // Procesa los atajos de teclado inspirados en Blender (0, Ctrl+0, Ctrl+Alt+0)
    void Actualizar(GestorEntidades& gestorEntidades, EntidadID entidadSeleccionadaID) {
        bool teclaCero = Entrada::EstaPresionadaTeclaClave('0');
        bool ctrlPresionado = Entrada::EstaPresionadaTeclaClave('LCTRL') || Entrada::EstaPresionadaTeclaClave('RCTRL');
        bool altPresionado = Entrada::EstaPresionadaTeclaClave('LALT') || Entrada::EstaPresionadaTeclaClave('RALT');

        // Ctrl + Alt + 0: Alinear Cámara Activa a la Vista actual de la entidad seleccionada
        if (ctrlPresionado && altPresionado && teclaCero) {
            if (m_CamaraActivaID != 0) {
                ComponenteTransformacion* transCamara = gestorEntidades.ObtenerComponente<ComponenteTransformacion>(m_CamaraActivaID);
                ComponenteTransformacion* transSeleccionada = gestorEntidades.ObtenerComponente<ComponenteTransformacion>(entidadSeleccionadaID);

                if (transCamara && transSeleccionada) {
                    transCamara->Posicion = transSeleccionada->Posicion;
                    transCamara->Rotacion = transSeleccionada->Rotacion;
                    std::cout << "[SistemaControlCamara] Ctrl + Alt + 0: Cámara alineada a la vista actual.\n";
                }
            }
        }
        // Ctrl + 0: Establecer la entidad seleccionada como Cámara Activa
        else if (ctrlPresionado && teclaCero) {
            if (entidadSeleccionadaID != 0) {
                EstablecerCamaraActiva(entidadSeleccionadaID);
            }
        }
        // 0: Alternar o enfocar a Vista de Cámara
        else if (teclaCero && !ctrlPresionado && !altPresionado) {
            if (m_CamaraActivaID != 0) {
                std::cout << "[SistemaControlCamara] 0: Cambiando a Vista de Cámara activa.\n";
                // Lógica de proyección de cámara para el renderizado gráfico
            }
        }
    }
};
