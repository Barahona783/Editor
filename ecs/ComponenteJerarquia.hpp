#pragma once
#include <vector>
#include <cstdint>

// ==========================================
// COMPONENTE DE JERARQUÍA (Grafo de Escena ECS)
// ==========================================
struct ComponenteJerarquia {
    uint32_t PadreID;             // 0 o ID nulo significa que es un nodo raíz en el mundo
    std::vector<uint32_t> HijosIDs; // Colección de IDs de las entidades hijas asociadas

    ComponenteJerarquia(uint32_t padreID = 0) 
        : PadreID(padreID) {}

    // Verifica si la entidad cuenta con un nodo padre asignado en la jerarquía
    bool TienePadre() const {
        return PadreID != 0;
    }

    // Añade un hijo a la jerarquía local si no está ya registrado
    void AgregarHijo(uint32_t hijoID) {
        for (uint32_t id : HijosIDs) {
            if (id == hijoID) return;
        }
        HijosIDs.push_back(hijoID);
    }

    // Remueve un hijo de la lista jerárquica
    void RemoverHijo(uint32_t hijoID) {
        for (auto it = HijosIDs.begin(); it != HijosIDs.end(); ++it) {
            if (*it == hijoID) {
                HijosIDs.erase(it);
                break;
            }
        }
    }
};
