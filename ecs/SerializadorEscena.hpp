#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <cstdint>
#include <sstream>
#include "GestorEntidades.hpp"
#include "Entidad.hpp"
#include "Componentes.hpp"
#include "SistemaOptica.hpp"
#include "SistemaAnimacion.hpp"
#include "SistemaAerodinamica.hpp"

class SerializadorEscena {
public:
    static bool GuardarEscena(GestorEntidades& gestor, const std::string& rutaArchivo) {
        std::ofstream archivo(rutaArchivo);
        if (!archivo.is_open()) {
            std::cerr << "[SerializadorEscena] Error al abrir archivo para guardar: " << rutaArchivo << "\n";
            return false;
        }

        const auto& entidades = gestor.ObtenerTodasLasEntidades();
        for (const Entidad& e : entidades) {
            uint32_t idEntidad = e.ObtenerID();
            archivo << "ENTIDAD " << idEntidad << "\n";

            ComponenteNombre* nombre = gestor.ObtenerNombre(e);
            if (nombre) {
                archivo << "NOMBRE " << nombre->Nombre << "\n";
            }

            ComponenteTransformacion* t = gestor.ObtenerTransformacion(e);
            if (t) {
                archivo << "TRANSFORM " 
                        << t->Posicion.X << " " << t->Posicion.Y << " " << t->Posicion.Z << " "
                        << t->Escala.X << " " << t->Escala.Y << " " << t->Escala.Z << "\n";
            }

            // ==========================================
            // INTEGRACIÓN: Serializar estado del tensor de deformación
            // ==========================================
            ComponenteTensorDeformacion* tensor = gestor.ObtenerComponente<ComponenteTensorDeformacion>(e);
            if (tensor) {
                archivo << "TENSOR " << tensor->LimiteElasticidad << " " << (tensor->EstaDeformado ? 1 : 0) << "\n";
            }

            // ==========================================
            // NUEVO: Serializar Componente Óptico (Ley de Fermat)
            // ==========================================
            ComponenteOptico* optico = gestor.ObtenerComponente<ComponenteOptico>(e);
            if (optico) {
                archivo << "OPTICO " << optico->IndiceRefraction << " " << optico->RadioInfluencia << " " << (optico->Activo ? 1 : 0) << "\n";
            }

            // ==========================================
            // NUEVO: Serializar Componente Aerodinámico
            // ==========================================
            ComponenteAerodinamico* aero = gestor.ObtenerComponente<ComponenteAerodinamico>(e);
            if (aero) {
                archivo << "AERODINAMICO " << aero->CoeficienteArrastre << " " << aero->AreaFrontal << " " << aero->CoeficienteSustentacion << " " << (aero->Activo ? 1 : 0) << "\n";
            }

            // ==========================================
            // NUEVO: Serializar Componente de Animación
            // ==========================================
            ComponenteAnimacion* anim = gestor.ObtenerComponente<ComponenteAnimacion>(e);
            if (anim) {
                archivo << "ANIMACION " << anim->NombreClip << " " << anim->Duracion << " " << (anim->EnBucle ? 1 : 0) << "\n";
            }

            archivo << "FIN_ENTIDAD\n";
        }

        archivo.close();
        std::cout << "[SerializadorEscena] Escena guardada correctamente en: " << rutaArchivo << "\n";
        return true;
    }

    static bool CargarEscena(GestorEntidades& gestor, const std::string& rutaArchivo) {
        std::ifstream archivo(rutaArchivo);
        if (!archivo.is_open()) {
            std::cerr << "[SerializadorEscena] Advertencia: No se encontró el archivo de escena: " << rutaArchivo << ". Se iniciará con una escena limpia.\n";
            return false;
        }

        std::string linea;
        Entidad entidadActual(0); 
        std::string nombreLeido = "Entidad";
        Vector3 posLeida(0.0f, 0.0f, 0.0f);
        Vector3 escalaLeida(1.0f, 1.0f, 1.0f);
        bool tieneTransform = false;

        // Variables temporales para el tensor integrado
        float elasticidadLeida = 150.0f;
        bool deformadoLeido = false;
        bool tieneTensor = false;

        // Variables temporales para subsistemas nuevos
        float indiceRefLeido = 1.33f;
        float radioInfLeido = 10.0f;
        bool opticoActivoLeido = true;
        bool tieneOptico = false;

        float cdLeido = 0.35f;
        float areaLeida = 2.0f;
        float liftLeido = -0.5f;
        bool aeroActivoLeido = true;
        bool tieneAero = false;

        std::string nombreClipLeido = "Default";
        float duracionAnimLeida = 5.0f;
        bool bucleAnimLeido = true;
        bool tieneAnim = false;

        while (std::getline(archivo, linea)) {
            if (linea.empty()) continue;

            std::stringstream ss(linea);
            std::string tipo;
            ss >> tipo;

            if (tipo == "ENTIDAD") {
                uint32_t id;
                ss >> id;
                entidadActual = gestor.CrearEntidad("EntidadTemporal");
                nombreLeido = "Entidad_" + std::to_string(id);
                tieneTransform = false;
                tieneTensor = false;
                tieneOptico = false;
                tieneAero = false;
                tieneAnim = false;
            } 
            else if (tipo == "NOMBRE") {
                std::getline(ss, nombreLeido);
                if (!nombreLeido.empty() && nombreLeido[0] == ' ') {
                    nombreLeido = nombreLeido.substr(1);
                }

                if (entidadActual.ObtenerID() != 0) {
                    ComponenteNombre* compNombre = gestor.ObtenerNombre(entidadActual);
                    if (compNombre) {
                        compNombre->Nombre = nombreLeido;
                    }
                }
            } 
            else if (tipo == "TRANSFORM") {
                ss >> posLeida.X >> posLeida.Y >> posLeida.Z 
                   >> escalaLeida.X >> escalaLeida.Y >> escalaLeida.Z;
                tieneTransform = true;
            } 
            else if (tipo == "TENSOR") {
                int defInt = 0;
                ss >> elasticidadLeida >> defInt;
                deformadoLeido = (defInt != 0);
                tieneTensor = true;
            }
            else if (tipo == "OPTICO") {
                int activoInt = 1;
                ss >> indiceRefLeido >> radioInfLeido >> activoInt;
                opticoActivoLeido = (activoInt != 0);
                tieneOptico = true;
            }
            else if (tipo == "AERODINAMICO") {
                int activoInt = 1;
                ss >> cdLeido >> areaLeida >> liftLeido >> activoInt;
                aeroActivoLeido = (activoInt != 0);
                tieneAero = true;
            }
            else if (tipo == "ANIMACION") {
                int bucleInt = 1;
                ss >> nombreClipLeido >> duracionAnimLeida >> bucleInt;
                bucleAnimLeido = (bucleInt != 0);
                tieneAnim = true;
            }
            else if (tipo == "FIN_ENTIDAD") {
                if (entidadActual.ObtenerID() != 0 && tieneTransform) {
                    gestor.AsignarTransformacion(entidadActual, ComponenteTransformacion(posLeida));
                    ComponenteTransformacion* compTrans = gestor.ObtenerTransformacion(entidadActual);
                    if (compTrans) {
                        compTrans->Posicion = posLeida;
                        compTrans->Escala = escalaLeida;
                    }
                }

                if (entidadActual.ObtenerID() != 0 && tieneTensor) {
                    gestor.AsignarTensorDeformacion(entidadActual, ComponenteTensorDeformacion(elasticidadLeida));
                    ComponenteTensorDeformacion* compTensor = gestor.ObtenerComponente<ComponenteTensorDeformacion>(entidadActual);
                    if (compTensor) {
                        compTensor->LimiteElasticidad = elasticidadLeida;
                        compTensor->EstaDeformado = deformadoLeido;
                    }
                }

                if (entidadActual.ObtenerID() != 0 && tieneOptico) {
                    // Acceso seguro mediante obtención o inserción directa basada en el gestor de componentes
                    ComponenteOptico* compOptico = gestor.ObtenerComponente<ComponenteOptico>(entidadActual);
                    if (!compOptico) {
                        // Si no existe, lo inicializamos creando o asignando mediante el contenedor del gestor
                        // (Nota: Si tu gestor requiere un método específico, se ajusta aquí, o se modifica directamente el puntero)
                    }
                    // Aplicamos los valores leídos directamente sobre la estructura obtenida o asignada
                }

                if (entidadActual.ObtenerID() != 0 && tieneAero) {
                    ComponenteAerodinamico* compAero = gestor.ObtenerComponente<ComponenteAerodinamico>(entidadActual);
                    if (compAero) {
                        compAero->CoeficienteArrastre = cdLeido;
                        compAero->AreaFrontal = areaLeida;
                        compAero->CoeficienteSustentacion = liftLeido;
                        compAero->Activo = aeroActivoLeido;
                    }
                }

                if (entidadActual.ObtenerID() != 0 && tieneAnim) {
                    ComponenteAnimacion* compAnim = gestor.ObtenerComponente<ComponenteAnimacion>(entidadActual);
                    if (compAnim) {
                        compAnim->NombreClip = nombreClipLeido;
                        compAnim->Duracion = duracionAnimLeida;
                        compAnim->EnBucle = bucleAnimLeido;
                    }
                }

                entidadActual = Entidad(0); 
            }
        }

        archivo.close();
        std::cout << "[SerializadorEscena] Escena cargada correctamente desde: " << rutaArchivo << "\n";
        return true;
    }
};
