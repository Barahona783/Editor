#pragma once
#include <vector>
#include <cstdint>

struct ComponenteJerarquia {
    uint32_t PadreID; // 0 o ID nulo significa que es un nodo raíz
    std::vector<uint32_t> HijosIDs;

    ComponenteJerarquia(uint32_t padreID = 0) 
        : PadreID(padreID) {}

    bool TienePadre() const {
        return PadreID != 0;
    }
};
