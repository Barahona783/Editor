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

    // ==========================================
    // ALMACENAMIENTO TENSORIAL Y FÍSICO AVANZADO
    // ==========================================
    std::unordered_map<IdentificadorEntidad, ComponenteCuerpoRigido> m_ComponentesCuerpoRigido;
    std::unordered_map<IdentificadorEntidad, ComponenteTensorDeformacion> m_ComponentesTensorDeformacion;
    std::unordered_map<IdentificadorEntidad, ComponenteInerciaRotacional> m_ComponentesInerciaRotacional;
    std::unordered_map<IdentificadorEntidad, ComponenteRestriccionFisica> m_ComponentesRestriccionFisica;
    std::unordered_map<IdentificadorEntidad, ComponenteAerodinamico> m_ComponentesAerodinamica;

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

    void AsignarCuerpoRigido(const Entidad& entidad, const ComponenteCuerpoRigido& cuerpo) {
        m_ComponentesCuerpoRigido[entidad.ObtenerID()] = cuerpo;
    }

    void AsignarTensorDeformacion(const Entidad& entidad, const ComponenteTensorDeformacion& tensor) {
        m_ComponentesTensorDeformacion[entidad.ObtenerID()] = tensor;
    }

    void AsignarInerciaRotacional(const Entidad& entidad, const ComponenteInerciaRotacional& inercia) {
        m_ComponentesInerciaRotacional[entidad.ObtenerID()] = inercia;
    }

    void AsignarRestriccionFisica(const Entidad& entidad, const ComponenteRestriccionFisica& restriccion) {
        m_ComponentesRestriccionFisica[entidad.ObtenerID()] = restriccion;
    }

    void AsignarAerodinamica(const Entidad& entidad, const ComponenteAerodinamico& aero) {
        m_ComponentesAerodinamica[entidad.ObtenerID()] = aero;
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

    ComponenteCuerpoRigido* ObtenerCuerpoRigido(const Entidad& entidad) {
        auto iter = m_ComponentesCuerpoRigido.find(entidad.ObtenerID());
        return (iter != m_ComponentesCuerpoRigido.end()) ? &(iter->second) : nullptr;
    }

    ComponenteTensorDeformacion* ObtenerTensorDeformacion(const Entidad& entidad) {
        auto iter = m_ComponentesTensorDeformacion.find(entidad.ObtenerID());
        return (iter != m_ComponentesTensorDeformacion.end()) ? &(iter->second) : nullptr;
    }

    ComponenteInerciaRotacional* ObtenerInerciaRotacional(const Entidad& entidad) {
        auto iter = m_ComponentesInerciaRotacional.find(entidad.ObtenerID());
        return (iter != m_ComponentesInerciaRotacional.end()) ? &(iter->second) : nullptr;
    }

    ComponenteRestriccionFisica* ObtenerRestriccionFisica(const Entidad& entidad) {
        auto iter = m_ComponentesRestriccionFisica.find(entidad.ObtenerID());
        return (iter != m_ComponentesRestriccionFisica.end()) ? &(iter->second) : nullptr;
    }

    ComponenteAerodinamico* ObtenerAerodinamica(const Entidad& entidad) {
        auto iter = m_ComponentesAerodinamica.find(entidad.ObtenerID());
        return (iter != m_ComponentesAerodinamica.end()) ? &(iter->second) : nullptr;
    }

    // Método genérico para compatibilidad con sistemas avanzados
    template <typename T>
    T* ObtenerComponente(const Entidad& entidad);

    const std::vector<Entidad>& ObtenerTodasLasEntidades() const {
        return m_Entidades;
    }
};

// Especializaciones del método genérico para los componentes avanzados y tensoriales
template <>
inline ComponenteTensorDeformacion* GestorEntidades::ObtenerComponente<ComponenteTensorDeformacion>(const Entidad& entidad) {
    return ObtenerTensorDeformacion(entidad);
}

template <>
inline ComponenteCuerpoRigido* GestorEntidades::ObtenerComponente<ComponenteCuerpoRigido>(const Entidad& entidad) {
    return ObtenerCuerpoRigido(entidad);
}

template <>
inline ComponenteInerciaRotacional* GestorEntidades::ObtenerComponente<ComponenteInerciaRotacional>(const Entidad& entidad) {
    return ObtenerInerciaRotacional(entidad);
}

template <>
inline ComponenteRestriccionFisica* GestorEntidades::ObtenerComponente<ComponenteRestriccionFisica>(const Entidad& entidad) {
    return ObtenerRestriccionFisica(entidad);
}

template <>
inline ComponenteAerodinamico* GestorEntidades::ObtenerComponente<ComponenteAerodinamico>(const Entidad& entidad) {
    return ObtenerAerodinamica(entidad);
}
