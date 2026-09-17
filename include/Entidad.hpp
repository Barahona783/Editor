#pragma once
#include <cstddef>

using IdentificadorEntidad = std::size_t;

class Entidad {
private:
    IdentificadorEntidad m_ID;

public:
    explicit Entidad(IdentificadorEntidad id) : m_ID(id) {}

    IdentificadorEntidad ObtenerID() const {
        return m_ID;
    }

    bool operator==(const Entidad& otra) const {
        return m_ID == otra.m_ID;
    }
};
