#pragma once
#include "Entidad.hpp"
#include "Componentes.hpp"
#include "Malla.hpp"
#include <vector>
#include <unordered_map>
#include <memory>
#include <iostream>

class GestorEntidades {
private:
    IdentificadorEntidad m_ContadorEntidades = 0;
    std::vector<Entidad> m_Entidades;

    // Almacenamiento de componentes por ID
    std::unordered_map<IdentificadorEntidad, ComponenteTransformacion> m_ComponentesTransformacion;
    std::unordered_map<IdentificadorEntidad, ComponenteNombre> m_ComponentesNombre;
    std::unordered_map<IdentificadorEntidad, ComponenteMalla> m_ComponentesMalla;
    std::unordered_map<IdentificadorEntidad, ComponenteColisionador> m_ComponentesColision;

public:
    Entidad CrearEntidad(const std::string& nombre = "ObjetoMotor") {
        IdentificadorEntidad nuevoID = ++m_ContadorEntidades;
        Entidad entidad(nuevoID);
        m_Entidades.push_back(entidad);

        m_ComponentesTransformacion[nuevoID] = ComponenteTransformacion();
        m_ComponentesNombre[nuevoID] = ComponenteNombre(nombre);

        return entidad;
    }

    // --- Métodos de Asignación ---
    void AsignarTransformacion(const Entidad& entidad, const ComponenteTransformacion& trans) {
        m_ComponentesTransformacion[entidad.ObtenerID()] = trans;
    }

    void AsignarMalla(const Entidad& entidad, std::shared_ptr<Malla> malla) {
        m_ComponentesMalla[entidad.ObtenerID()] = ComponenteMalla(malla);
    }

    void AsignarCajaColision(const Entidad& entidad, const CajaColision& caja) {
        m_ComponentesColision[entidad.ObtenerID()] = ComponenteColisionador(caja);
    }

    // --- Métodos de Obtención ---
    ComponenteTransformacion* ObtenerTransformacion(const Entidad& entidad) {
        auto iter = m_ComponentesTransformacion.find(entidad.ObtenerID());
        return (iter != m_ComponentesTransformacion.end()) ? &(iter->second) : nullptr;
    }

    ComponenteNombre* ObtenerNombre(const Entidad& entidad) {
        auto iter = m_ComponentesNombre.find(entidad.ObtenerID());
        return (iter != m_ComponentesNombre.end()) ? &(iter->second) : nullptr;
    }

    ComponenteColisionador* ObtenerCajaColision(const Entidad& entidad) {
        auto iter = m_ComponentesColision.find(entidad.ObtenerID());
        return (iter != m_ComponentesColision.end()) ? &(iter->second) : nullptr;
    }

    const std::vector<Entidad>& ObtenerTodasLasEntidades() const {
        return m_Entidades;
    }
};
